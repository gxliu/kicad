/*
 * This program source code file is part of KiCad, a free EDA CAD application.
 *
 * Copyright (C) 2013 CERN
 * @author Maciej Suminski <maciej.suminski@cern.ch>
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

#include <class_board.h>
#include <class_module.h>
#include <class_zone.h>
#include <wxPcbStruct.h>
#include <tool/tool_manager.h>
#include <view/view_controls.h>
#include <confirm.h>
#include <cassert>

#include "common_actions.h"
#include "selection_tool.h"
#include "edit_tool.h"

using namespace KIGFX;
using boost::optional;

EDIT_TOOL::EDIT_TOOL() :
    TOOL_INTERACTIVE( "pcbnew.InteractiveEdit" ), m_selectionTool( NULL )
{
}


bool EDIT_TOOL::Init()
{
    // Find the selection tool, so they can cooperate
    TOOL_BASE* selectionTool = m_toolMgr->FindTool( "pcbnew.InteractiveSelection" );

    m_selectionTool = static_cast<SELECTION_TOOL*>( selectionTool );
    if( !selectionTool )
    {
        DisplayError( NULL, wxT( "pcbnew.InteractiveSelection tool is not available" ) );
        return false;
    }

    // Add context menu entries that are displayed when selection tool is active
    m_selectionTool->AddMenuItem( COMMON_ACTIONS::editActivate );
    m_selectionTool->AddMenuItem( COMMON_ACTIONS::rotate );
    m_selectionTool->AddMenuItem( COMMON_ACTIONS::flip );
    m_selectionTool->AddMenuItem( COMMON_ACTIONS::remove );
    m_selectionTool->AddMenuItem( COMMON_ACTIONS::properties );

    setTransitions();

    return true;
}


int EDIT_TOOL::Main( TOOL_EVENT& aEvent )
{
    const SELECTION_TOOL::SELECTION& selection = m_selectionTool->GetSelection();

    if( selection.Empty() )
        return 0; // there are no items to operate on

    VECTOR2D dragPosition;
    m_dragging = false;
    bool restore = false;       // Should items' state be restored when finishing the tool?

    VIEW_CONTROLS* controls = getViewControls();
    controls->ShowCursor( true );
    controls->SetSnapping( true );
    controls->SetAutoPan( true );

    // Main loop: keep receiving events
    while( OPT_TOOL_EVENT evt = Wait() )
    {
        if( evt->IsCancel() )
        {
            restore = true; // Cancelling the tool means that items have to be restored
            break;          // Finish
        }

        // Dispatch TOOL_ACTIONs
        else if( evt->Category() == TC_COMMAND )
        {
            if( evt->IsAction( &COMMON_ACTIONS::rotate ) )
                Rotate( aEvent );
            else if( evt->IsAction( &COMMON_ACTIONS::flip ) )
                Flip( aEvent );
        }

        else if( evt->IsMotion() || evt->IsDrag( BUT_LEFT ) )
        {
            if( m_dragging )
            {
                // Drag items to the current cursor position
                VECTOR2D movement = ( evt->Position() - dragPosition );
                m_state.Move( movement );
            }
            else
            {
                // Prepare to drag
                std::set<BOARD_ITEM*>::iterator it;

                for( it = selection.items.begin(); it != selection.items.end(); ++it )
                {
                    // Save the state of the selected items, in case it has to be restored
                    m_state.Save( *it );
                }

                m_dragging = true;
            }

            selection.group->ViewUpdate( VIEW_ITEM::GEOMETRY );
            dragPosition = evt->Position();
        }

        else if( evt->IsMouseUp( BUT_LEFT ) || evt->IsClick( BUT_LEFT ) )
            break; // Finish
    }

    m_dragging = false;

    if( restore )
    {
        // Modifications has to be rollbacked, so restore the previous state of items
        selection.group->ItemsViewUpdate( VIEW_ITEM::APPEARANCE );
        m_state.RestoreAll();
    }
    else
    {
        // Changes are applied, so update the items
        selection.group->ItemsViewUpdate( m_state.GetUpdateFlag() );
        m_state.Apply();
    }

    controls->ShowCursor( false );
    controls->SetSnapping( false );
    controls->SetAutoPan( false );

    setTransitions();

    return 0;
}


int EDIT_TOOL::Properties( TOOL_EVENT& aEvent )
{
    const SELECTION_TOOL::SELECTION& selection = m_selectionTool->GetSelection();

    // Properties are displayed when there is only one item selected
    if( selection.items.size() == 1 )
    {
        // Display properties dialog
        PCB_EDIT_FRAME* editFrame = static_cast<PCB_EDIT_FRAME*>( m_toolMgr->GetEditFrame() );
        BOARD_ITEM* item = *selection.items.begin();
        editFrame->OnEditItemRequest( NULL, item );

        item->ViewUpdate( KIGFX::VIEW_ITEM::GEOMETRY );
    }

    setTransitions();

    return 0;
}


int EDIT_TOOL::Rotate( TOOL_EVENT& aEvent )
{
    const SELECTION_TOOL::SELECTION& selection = m_selectionTool->GetSelection();
    VECTOR2D cursorPos = getView()->ToWorld( getViewControls()->GetCursorPosition() );

    if( m_dragging )
    {
        m_state.Rotate( cursorPos, 900.0 );
        selection.group->ViewUpdate( VIEW_ITEM::GEOMETRY );
    }
    else
    {
        std::set<BOARD_ITEM*>::iterator it;

        for( it = selection.items.begin(); it != selection.items.end(); ++it )
        {
            (*it)->Rotate( wxPoint( cursorPos.x, cursorPos.y ), 900.0 );
            (*it)->ViewUpdate( KIGFX::VIEW_ITEM::GEOMETRY );
        }

        setTransitions();
    }

    return 0;
}


int EDIT_TOOL::Flip( TOOL_EVENT& aEvent )
{
    const SELECTION_TOOL::SELECTION& selection = m_selectionTool->GetSelection();
    VECTOR2D cursorPos = getView()->ToWorld( getViewControls()->GetCursorPosition() );

    if( m_dragging )
    {
        m_state.Flip( cursorPos );
        selection.group->ViewUpdate( VIEW_ITEM::GEOMETRY );
    }
    else
    {
        std::set<BOARD_ITEM*>::iterator it;

        for( it = selection.items.begin(); it != selection.items.end(); ++it )
        {
            (*it)->Flip( wxPoint( cursorPos.x, cursorPos.y ) );
            (*it)->ViewUpdate( KIGFX::VIEW_ITEM::LAYERS );
        }

        setTransitions();
    }

    return 0;
}


int EDIT_TOOL::Remove( TOOL_EVENT& aEvent )
{
    // Get a copy of the selected items set
    std::set<BOARD_ITEM*> selectedItems = m_selectionTool->GetSelection().items;

    // As we are about to remove items, they have to be removed from the selection
    m_selectionTool->ClearSelection();

    std::set<BOARD_ITEM*>::iterator it;
    for( it = selectedItems.begin(); it != selectedItems.end(); ++it )
        remove( *it );

    BOARD* board = getModel<BOARD>( PCB_T );
    // Rebuild list of pads and nets if necessary
    if( !( board->GetStatus() & NET_CODES_OK ) )
        board->BuildListOfNets();

    setTransitions();

    return 0;
}


void EDIT_TOOL::remove( BOARD_ITEM* aItem )
{
    BOARD* board = getModel<BOARD>( PCB_T );

    switch( aItem->Type() )
    {
    case PCB_MODULE_T:
    {
        MODULE* module = static_cast<MODULE*>( aItem );

        for( D_PAD* pad = module->Pads().GetFirst(); pad; pad = pad->Next() )
            getView()->Remove( pad );

        for( BOARD_ITEM* drawing = module->GraphicalItems().GetFirst(); drawing;
             drawing = drawing->Next() )
            getView()->Remove( drawing );

        getView()->Remove( &module->Reference() );
        getView()->Remove( &module->Value() );

        // Module itself is deleted after the switch scope
        // list of pads is rebuild by BOARD::BuildListOfNets()

//        module->ClearFlags();       // TODO is it necessary? clearing ratsnest/list of pads?
        // Clear flags to indicate, that the ratsnest, list of nets & pads are not valid anymore
        board->m_Status_Pcb = 0;
    }
    break;

    case PCB_ZONE_AREA_T:
        getView()->Remove( aItem );
        getModel<BOARD>( PCB_T )->Delete( aItem );
        return;

    // These are not supposed to be removed
    case PCB_PAD_T:
    case PCB_MODULE_TEXT_T:
    case PCB_MODULE_EDGE_T:
        return;

    case PCB_LINE_T:                // a segment not on copper layers
    case PCB_TEXT_T:                // a text on a layer
    case PCB_TRACE_T:               // a track segment (segment on a copper layer)
    case PCB_VIA_T:                 // a via (like track segment on a copper layer)
    case PCB_DIMENSION_T:           // a dimension (graphic item)
    case PCB_TARGET_T:              // a target (graphic item)
    case PCB_MARKER_T:              // a marker used to show something
    case PCB_ZONE_T:                // SEG_ZONE items are now deprecated
        break;

    // TODO
    default:                        // other types do not need to (or should not) be handled
        assert( false );
        return;
        break;
    }

    getView()->Remove( aItem );
    board->Delete( aItem );
}


void EDIT_TOOL::setTransitions()
{
    Go( &EDIT_TOOL::Main,       COMMON_ACTIONS::editActivate.MakeEvent() );
    Go( &EDIT_TOOL::Rotate,     COMMON_ACTIONS::rotate.MakeEvent() );
    Go( &EDIT_TOOL::Flip,       COMMON_ACTIONS::flip.MakeEvent() );
    Go( &EDIT_TOOL::Remove,     COMMON_ACTIONS::remove.MakeEvent() );
    Go( &EDIT_TOOL::Properties, COMMON_ACTIONS::properties.MakeEvent() );
}
