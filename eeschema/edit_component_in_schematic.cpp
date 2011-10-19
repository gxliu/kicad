/*
 * This program source code file is part of KiCad, a free EDA CAD application.
 *
 * Copyright (C) 2004 Jean-Pierre Charras, jaen-pierre.charras@gipsa-lab.inpg.com
 * Copyright (C) 2008-2011 Wayne Stambaugh <stambaughw@verizon.net>
 * Copyright (C) 2004-2011 KiCad Developers, see change_log.txt for contributors.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, you may find one here:
 * http://www.gnu.org/licenses/old-licenses/gpl-2.0.html
 * or you may search the http://www.gnu.org website for the version 2 license,
 * or you may write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
 */

/**
 * @file edit_component_in_schematic.cpp
 * @brief Schematic component editing code.
 */

#include "fctsys.h"
#include "gr_basic.h"
#include "class_drawpanel.h"
#include "confirm.h"
#include "wxEeschemaStruct.h"

#include "general.h"
#include "protos.h"
#include "class_library.h"
#include "sch_component.h"


/*
 * Move standard text field.  This routine is normally attached to the cursor.
 */
static void moveField( EDA_DRAW_PANEL* aPanel, wxDC* aDC, const wxPoint& aPosition, bool aErase )
{
    wxPoint pos;

    SCH_EDIT_FRAME* frame = (SCH_EDIT_FRAME*) aPanel->GetParent();
    SCH_SCREEN* screen = (SCH_SCREEN*) aPanel->GetScreen();
    SCH_FIELD* currentField = (SCH_FIELD*)screen->GetCurItem();

    if( (currentField == NULL) || (currentField->Type() != SCH_FIELD_T) )
        return;

    SCH_COMPONENT* component = (SCH_COMPONENT*) currentField->GetParent();

    currentField->m_AddExtraText = frame->m_Multiflag;

    if( aErase )
    {
        currentField->Draw( aPanel, aDC, wxPoint( 0, 0 ), g_XorMode );
    }

    pos = ( (SCH_COMPONENT*) currentField->GetParent() )->GetPosition();

    // Actual positions are calculated by the rotation/mirror transform
    // But here we want the relative position of the moved field
    // and we know the actual position.
    // So we are using the inverse rotation/mirror transform.
    wxPoint pt( screen->GetCrossHairPosition() - pos );

    TRANSFORM itrsfm = component->GetTransform().InverseTransform();
    currentField->SetPosition( pos + itrsfm.TransformCoordinate( pt ) );

    currentField->Draw( aPanel, aDC, wxPoint( 0, 0 ), g_XorMode );
}


static void abortMoveField( EDA_DRAW_PANEL* aPanel, wxDC* aDC )
{
    SCH_EDIT_FRAME* frame = (SCH_EDIT_FRAME*) aPanel->GetParent();
    SCH_SCREEN* screen = (SCH_SCREEN*) aPanel->GetScreen();
    SCH_FIELD* currentField = (SCH_FIELD*) screen->GetCurItem();

    if( currentField )
    {
        currentField->m_AddExtraText = frame->m_Multiflag;
        currentField->Draw( aPanel, aDC, wxPoint( 0, 0 ), g_XorMode );
        currentField->ClearFlags();
        currentField->m_Pos = frame->m_OldPos;
        currentField->Draw( aPanel, aDC, wxPoint( 0, 0 ), GR_DEFAULT_DRAWMODE );
    }

    screen->SetCurItem( NULL );
}


void SCH_EDIT_FRAME::MoveField( SCH_FIELD* aField, wxDC* aDC )
{
    wxCHECK_RET( aField && (aField->Type() == SCH_FIELD_T) && !aField->GetText().IsEmpty(),
                 wxT( "Cannot move invalid component field." ) );

    LIB_COMPONENT* libEntry;
    wxPoint        pos, newpos;
    SCH_COMPONENT* comp = (SCH_COMPONENT*) aField->GetParent();

    GetScreen()->SetCurItem( aField );
    SetUndoItem( comp );

    pos = comp->GetPosition();

    /* Positions are computed by the rotation/mirror transform. */
    newpos = aField->m_Pos - pos;

    newpos = comp->GetTransform().TransformCoordinate( newpos ) + pos;

    DrawPanel->CrossHairOff( aDC );
    GetScreen()->SetCrossHairPosition( newpos );
    DrawPanel->MoveCursorToCrossHair();

    m_OldPos = aField->m_Pos;
    m_Multiflag = 0;

    if( aField->GetId() == REFERENCE )
    {
        libEntry = CMP_LIBRARY::FindLibraryComponent( comp->GetLibName() );

        if( (libEntry != NULL) && (libEntry->GetPartCount() > 1) )
            m_Multiflag = 1;
    }

    DrawPanel->SetMouseCapture( moveField, abortMoveField );
    aField->SetFlags( IS_MOVED );

    DrawPanel->CrossHairOn( aDC );
}


void SCH_EDIT_FRAME::EditComponentFieldText( SCH_FIELD* aField, wxDC* aDC )
{
    wxCHECK_RET( aField != NULL && aField->Type() == SCH_FIELD_T,
                 wxT( "Cannot edit invalid schematic field." ) );

    int            fieldNdx, flag;
    SCH_COMPONENT* component = (SCH_COMPONENT*) aField->GetParent();

    wxCHECK_RET( component != NULL && component->Type() == SCH_COMPONENT_T,
                 wxT( "Invalid schematic field parent item." ) );

    LIB_COMPONENT* entry = CMP_LIBRARY::FindLibraryComponent( component->GetLibName() );

    wxCHECK_RET( entry != NULL, wxT( "Library entry for component <" ) +
                 component->GetLibName() + wxT( "> could not be found." ) );

    fieldNdx = aField->GetId();

    if( fieldNdx == VALUE && entry->IsPower() )
    {
        wxString msg;
        msg.Printf( _( "%s is a power component and it's value cannot be modified!\n\nYou must \
create a new power component with the new value." ), GetChars( entry->GetName() ) );
        DisplayInfoMessage( this, msg );
        return;
    }

    flag = 0;

    if( fieldNdx == REFERENCE && entry->GetPartCount() > 1 )
        flag = 1;

    // Save old component in undo list if not already in edit, or moving.
    if( aField->GetFlags() == 0 )
        SaveCopyInUndoList( component, UR_CHANGED );

    wxString newtext = aField->m_Text;
    DrawPanel->m_IgnoreMouseEvents = true;

    wxString title;
    title.Printf( _( "Edit %s Field" ), GetChars( aField->m_Name ) );

    wxTextEntryDialog dlg( this, wxEmptyString , title, newtext );
    int response = dlg.ShowModal();

    DrawPanel->MoveCursorToCrossHair();
    DrawPanel->m_IgnoreMouseEvents = false;
    newtext = dlg.GetValue( );
    newtext.Trim( true );
    newtext.Trim( false );

    if ( response != wxID_OK || newtext == aField->GetText() )
        return;  // canceled by user

    aField->m_AddExtraText = flag;
    aField->Draw( DrawPanel, aDC, wxPoint( 0, 0 ), g_XorMode );

    if( !newtext.IsEmpty() )
    {
        if( aField->m_Text.IsEmpty() )  // Means the field was not already in use
        {
            aField->m_Pos = component->GetPosition();
            aField->m_Size.x = aField->m_Size.y = m_TextFieldSize;
        }

        if( fieldNdx == REFERENCE )
        {
            // Test is reference is acceptable:
            if( SCH_COMPONENT::IsReferenceStringValid( newtext ) )
            {
                component->SetRef( GetSheet(), newtext );
                aField->m_Text = newtext;
            }
            else
            {
                DisplayError( this, _( "Illegal reference string!  No change" ) );
            }
        }
        else
            aField->m_Text = newtext;
    }
    else
    {
        if( fieldNdx == REFERENCE )
        {
            DisplayError( this, _( "The reference field cannot be empty!  No change" ) );
        }
        else if( fieldNdx == VALUE )
        {
            DisplayError( this, _( "The value field cannot be empty!  No change" ) );
        }
        else
        {
            aField->m_Text = wxT( "~" );
        }
    }

    aField->Draw( DrawPanel, aDC, wxPoint( 0, 0 ), g_XorMode );
    component->DisplayInfo( this );
    OnModify();
}


void SCH_EDIT_FRAME::RotateField( SCH_FIELD* aField, wxDC* aDC )
{
    wxCHECK_RET( aField != NULL && aField->Type() == SCH_FIELD_T && !aField->GetText().IsEmpty(),
                 wxT( "Cannot rotate invalid schematic field." ) );

    int            flag = 0;
    LIB_COMPONENT* libEntry;
    SCH_COMPONENT* component = (SCH_COMPONENT*) aField->GetParent();

    if( aField->GetId() == REFERENCE )
    {
        libEntry = CMP_LIBRARY::FindLibraryComponent( component->GetLibName() );

        if( (libEntry != NULL) && (libEntry->GetPartCount() > 1) )
            flag = 1;
    }

    // Save old component in undo list if not already in edit, or moving.
    if( aField->GetFlags() == 0 )
        SaveCopyInUndoList( component, UR_CHANGED );

    aField->m_AddExtraText = flag;
    aField->Draw( DrawPanel, aDC, wxPoint( 0, 0 ), g_XorMode );

    if( aField->m_Orient == TEXT_ORIENT_HORIZ )
        aField->m_Orient = TEXT_ORIENT_VERT;
    else
        aField->m_Orient = TEXT_ORIENT_HORIZ;

    aField->Draw( DrawPanel, aDC, wxPoint( 0, 0 ), g_XorMode );

    OnModify();
}
