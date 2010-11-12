
#include "fctsys.h"
#include "gr_basic.h"
#include "common.h"
#include "kicad_string.h"
#include "eeschema_id.h"
#include "appl_wxstruct.h"
#include "class_drawpanel.h"
#include "sch_item_struct.h"
#include "class_sch_screen.h"
#include "wxEeschemaStruct.h"

#include "general.h"
#include "protos.h"
#include "class_library.h"
#include "sch_items.h"
#include "sch_sheet.h"

#include <boost/foreach.hpp>


void SetaParent( EDA_BaseStruct* Struct, BASE_SCREEN* Screen )
{
    switch( Struct->Type() )
    {
    case DRAW_POLYLINE_STRUCT_TYPE:
    case DRAW_JUNCTION_STRUCT_TYPE:
    case TYPE_SCH_TEXT:
    case TYPE_SCH_LABEL:
    case TYPE_SCH_GLOBALLABEL:
    case TYPE_SCH_HIERLABEL:
    case TYPE_SCH_COMPONENT:
    case DRAW_SEGMENT_STRUCT_TYPE:
    case DRAW_BUSENTRY_STRUCT_TYPE:
    case DRAW_SHEET_STRUCT_TYPE:
    case TYPE_SCH_MARKER:
    case DRAW_NOCONNECT_STRUCT_TYPE:
        Struct->SetParent( Screen );
        break;

    case DRAW_HIERARCHICAL_PIN_SHEET_STRUCT_TYPE:
        break;

    default:
        break;
    }
}


/* Default EESchema zoom values. Limited to 17 values to keep a decent size
 * to menus
 */
static int SchematicZoomList[] =
{
    5, 7, 10, 15, 20, 30, 40, 60, 80, 120, 160, 230, 320, 480, 640, 800, 1280
};

#define SCHEMATIC_ZOOM_LIST_CNT  ( sizeof( SchematicZoomList ) / sizeof( int ) )
#define MM_TO_SCH_UNITS 1000.0 / 25.4       //schematic internal unites are mils


/* Default grid sizes for the schematic editor.
 * Do NOT add others values (mainly grid values in mm),
 * because they can break the schematic:
 * because wires and pins are considered as connected when the are to the same coordinate
 * we cannot mix coordinates in mils (internal units) and mm
 * (that cannot exactly converted in mils in many cases
 * in fact schematic must only use 50 and 25 mils to place labels, wires and components
 * others values are useful only for graphic items (mainly in library editor)
 * so use integer values in mils only.
*/
static GRID_TYPE SchematicGridList[] = {
    { ID_POPUP_GRID_LEVEL_50, wxRealPoint( 50, 50 ) },
    { ID_POPUP_GRID_LEVEL_25, wxRealPoint( 25, 25 ) },
    { ID_POPUP_GRID_LEVEL_10, wxRealPoint( 10, 10 ) },
    { ID_POPUP_GRID_LEVEL_5, wxRealPoint( 5, 5 ) },
    { ID_POPUP_GRID_LEVEL_2, wxRealPoint( 2, 2 ) },
    { ID_POPUP_GRID_LEVEL_1, wxRealPoint( 1, 1 ) },
};

#define SCHEMATIC_GRID_LIST_CNT ( sizeof( SchematicGridList ) / sizeof( GRID_TYPE ) )


SCH_SCREEN::SCH_SCREEN( KICAD_T type ) : BASE_SCREEN( type )
{
    size_t i;

    EEDrawList = NULL;                  /* Schematic items list */
    m_Zoom = 32;

    for( i = 0; i < SCHEMATIC_ZOOM_LIST_CNT; i++ )
        m_ZoomList.Add( SchematicZoomList[i] );

    for( i = 0; i < SCHEMATIC_GRID_LIST_CNT; i++ )
        AddGrid( SchematicGridList[i] );

    SetGrid( wxRealPoint( 50, 50 ) );   /* Default grid size. */
    m_RefCount = 0;
    m_Center = false;                   /* Suitable for schematic only. For
                                         * libedit and viewlib, must be set
                                         * to true */
    InitDatas();
}


SCH_SCREEN::~SCH_SCREEN()
{
    ClearUndoRedoList();
    FreeDrawList();
}


void SCH_SCREEN::FreeDrawList()
{
    SCH_ITEM* DrawStruct;

    while( EEDrawList != NULL )
    {
        DrawStruct = EEDrawList;
        EEDrawList = EEDrawList->Next();
        SAFE_DELETE( DrawStruct );
    }

    EEDrawList = NULL;
}


/* If found in EEDrawList, remove DrawStruct from EEDrawList.
 *  DrawStruct is not deleted or modified
 */
void SCH_SCREEN::RemoveFromDrawList( SCH_ITEM * DrawStruct )
{
    if( DrawStruct == EEDrawList )
        EEDrawList = EEDrawList->Next();
    else
    {
        EDA_BaseStruct* DrawList = EEDrawList;
        while( DrawList && DrawList->Next() )
        {
            if( DrawList->Next() == DrawStruct )
            {
                DrawList->SetNext( DrawList->Next()->Next() );
                break;
            }
            DrawList = DrawList->Next();
        }
    }
}


bool SCH_SCREEN::CheckIfOnDrawList( SCH_ITEM* st )
{
    SCH_ITEM * DrawList = EEDrawList;

    while( DrawList )
    {
        if( DrawList == st )
            return true;
        DrawList = DrawList->Next();
    }

    return false;
}


void SCH_SCREEN::AddToDrawList( SCH_ITEM* st )
{
    st->SetNext( EEDrawList );
    EEDrawList = st;
}


/* Extract the old wires, junctions and buses, an if CreateCopy replace them
 * by a copy.  Old ones must be put in undo list, and the new ones can be
 * modified by clean up safely.
 * If an abort command is made, old wires must be put in EEDrawList, and
 * copies must be deleted.  This is because previously stored undo commands
 * can handle pointers on wires or bus, and we do not delete wires or bus,
 * we must put they in undo list.
 *
 * Because cleanup delete and/or modify bus and wires, the more easy is to put
 * all wires in undo list and use a new copy of wires for cleanup.
 */
SCH_ITEM* SCH_SCREEN::ExtractWires( bool CreateCopy )
{
    SCH_ITEM* item, * next_item, * new_item, * List = NULL;

    for( item = EEDrawList; item != NULL; item = next_item )
    {
        next_item = item->Next();

        switch( item->Type() )
        {
        case DRAW_JUNCTION_STRUCT_TYPE:
        case DRAW_SEGMENT_STRUCT_TYPE:
            RemoveFromDrawList( item );
            item->SetNext( List );
            List = item;
            if( CreateCopy )
            {
                if( item->Type() == DRAW_JUNCTION_STRUCT_TYPE )
                    new_item = ( (SCH_JUNCTION*) item )->GenCopy();
                else
                    new_item = ( (SCH_LINE*) item )->GenCopy();
                new_item->SetNext( EEDrawList );
                EEDrawList = new_item;
            }
            break;

        default:
            break;
        }
    }

    return List;
}


/* Routine cleaning:
 * - Includes segments or buses aligned in only 1 segment
 * - Detects identical objects superimposed
 */
bool SCH_SCREEN::SchematicCleanUp( wxDC* DC )
{
    SCH_ITEM* DrawList, * TstDrawList;
    bool      Modify = FALSE;

    DrawList = EEDrawList;
    for( ; DrawList != NULL; DrawList = DrawList->Next() )
    {
        if( DrawList->Type() == DRAW_SEGMENT_STRUCT_TYPE )
        {
            TstDrawList = DrawList->Next();
            while( TstDrawList )
            {
                if( TstDrawList->Type() == DRAW_SEGMENT_STRUCT_TYPE )
                {
                    SCH_LINE* line = (SCH_LINE*) DrawList;

                    if( line->MergeOverlap( (SCH_LINE*) TstDrawList ) )
                    {
                        /* keep the bits set in .m_Flags, because the deleted
                         * segment can be flagged */
                        DrawList->m_Flags |= TstDrawList->m_Flags;
                        EraseStruct( TstDrawList, this );
                        SetRefreshReq();
                        TstDrawList = EEDrawList;
                        Modify = TRUE;
                    }
                    else
                    {
                        TstDrawList = TstDrawList->Next();
                    }
                }
                else
                {
                    TstDrawList = TstDrawList->Next();
                }
            }
        }
    }

    WinEDA_SchematicFrame* frame;
    frame = (WinEDA_SchematicFrame*) wxGetApp().GetTopWindow();
    frame->TestDanglingEnds( EEDrawList, DC );

    return Modify;
}


/**
 * Function Save
 * writes the data structures for this object out to a FILE in "*.sch" format.
 * @param aFile The FILE to write to.
 * @return bool - true if success writing else false.
 */
bool SCH_SCREEN::Save( FILE* aFile ) const
{
    // Creates header
    if( fprintf( aFile, "%s %s %d", EESCHEMA_FILE_STAMP,
                 SCHEMATIC_HEAD_STRING, EESCHEMA_VERSION ) < 0
        || fprintf( aFile, "  date %s\n", CONV_TO_UTF8( DateAndTime() ) ) < 0 )
        return FALSE;

    BOOST_FOREACH( const CMP_LIBRARY& lib, CMP_LIBRARY::GetLibraryList() )
    {
        if( fprintf( aFile, "LIBS:%s\n", CONV_TO_UTF8( lib.GetName() ) ) < 0 )
            return FALSE;
    }

    if( fprintf( aFile, "EELAYER %2d %2d\n", g_LayerDescr.NumberOfLayers,
                 g_LayerDescr.CurrentLayer ) < 0
        || fprintf( aFile, "EELAYER END\n" ) < 0 )
        return FALSE;

    /* Write page info, ScreenNumber and NumberOfScreen; not very meaningful for
     * SheetNumber and Sheet Count in a complex hierarchy, but useful in
     * simple hierarchy and flat hierarchy.  Used also to search the root
     * sheet ( ScreenNumber = 1 ) within the files
     */

    if( fprintf( aFile, "$Descr %s %d %d\n", CONV_TO_UTF8( m_CurrentSheetDesc->m_Name ),
                 m_CurrentSheetDesc->m_Size.x, m_CurrentSheetDesc->m_Size.y ) < 0
        || fprintf( aFile, "Sheet %d %d\n", m_ScreenNumber, m_NumberOfScreen ) < 0
        || fprintf( aFile, "Title \"%s\"\n", CONV_TO_UTF8( m_Title ) ) < 0
        || fprintf( aFile, "Date \"%s\"\n", CONV_TO_UTF8( m_Date ) ) < 0
        || fprintf( aFile, "Rev \"%s\"\n", CONV_TO_UTF8( m_Revision ) ) < 0
        || fprintf( aFile, "Comp \"%s\"\n", CONV_TO_UTF8( m_Company ) ) < 0
        || fprintf( aFile, "Comment1 \"%s\"\n", CONV_TO_UTF8( m_Commentaire1 ) ) < 0
        || fprintf( aFile, "Comment2 \"%s\"\n", CONV_TO_UTF8( m_Commentaire2 ) ) < 0
        || fprintf( aFile, "Comment3 \"%s\"\n", CONV_TO_UTF8( m_Commentaire3 ) ) < 0
        || fprintf( aFile, "Comment4 \"%s\"\n", CONV_TO_UTF8( m_Commentaire4 ) ) < 0
        || fprintf( aFile, "$EndDescr\n" ) < 0 )
        return FALSE;

    for( SCH_ITEM* item = EEDrawList; item; item = item->Next() )
    {
        if( !item->Save( aFile ) )
            return FALSE;
    }

    if( fprintf( aFile, "$EndSCHEMATC\n" ) < 0 )
        return FALSE;

    return TRUE;
}


/** Function ClearUndoORRedoList
 * free the undo or redo list from List element
 *  Wrappers are deleted.
 *  datas pointed by wrappers are deleted if not in use in schematic
 *  i.e. when they are copy of a schematic item or they are no more in use
 *  (DELETED)
 * @param aList = the UNDO_REDO_CONTAINER to clear
 * @param aItemCount = the count of items to remove. < 0 for all items
 * items (commands stored in list) are removed from the beginning of the list.
 * So this function can be called to remove old commands
 */
void SCH_SCREEN::ClearUndoORRedoList( UNDO_REDO_CONTAINER& aList, int aItemCount )
{
    if( aItemCount == 0 )
        return;

    unsigned icnt = aList.m_CommandsList.size();
    if( aItemCount > 0 )
        icnt = aItemCount;
    for( unsigned ii = 0; ii < icnt; ii++ )
    {
        if( aList.m_CommandsList.size() == 0 )
            break;

        PICKED_ITEMS_LIST* curr_cmd = aList.m_CommandsList[0];
        aList.m_CommandsList.erase( aList.m_CommandsList.begin() );

        curr_cmd->ClearListAndDeleteItems();
        delete curr_cmd;    // Delete command
    }
}


void SCH_SCREEN::ClearDrawingState()
{
    for( SCH_ITEM* item = EEDrawList; item != NULL; item = item->Next() )
        item->m_Flags = 0;
}


/******************************************************************/
/* Class SCH_SCREENS to handle the list of screens in a hierarchy */
/******************************************************************/

SCH_SCREENS::SCH_SCREENS()
{
    m_index = 0;
    BuildScreenList( g_RootSheet );
}


SCH_SCREENS::~SCH_SCREENS()
{
}


SCH_SCREEN* SCH_SCREENS::GetFirst()
{
    m_index = 0;

    if( m_screens.size() > 0 )
        return m_screens[0];

    return NULL;
}


SCH_SCREEN* SCH_SCREENS::GetNext()
{
    if( m_index < m_screens.size() )
        m_index++;

    return GetScreen( m_index );
}


SCH_SCREEN* SCH_SCREENS::GetScreen( unsigned int aIndex )
{
    if( aIndex < m_screens.size() )
        return m_screens[ aIndex ];

    return NULL;
}


void SCH_SCREENS::AddScreenToList( SCH_SCREEN* aScreen )
{
    if( aScreen == NULL )
        return;

    for( unsigned int i = 0; i < m_screens.size(); i++ )
    {
        if( m_screens[i] == aScreen )
            return;
    }

    m_screens.push_back( aScreen );
}


void SCH_SCREENS::BuildScreenList( EDA_BaseStruct* aItem )
{
    if( aItem && aItem->Type() == DRAW_SHEET_STRUCT_TYPE )
    {
        SCH_SHEET* ds = (SCH_SHEET*) aItem;
        aItem = ds->m_AssociatedScreen;
    }
    if( aItem && aItem->Type() == SCREEN_STRUCT_TYPE )
    {
        SCH_SCREEN*     screen = (SCH_SCREEN*) aItem;
        AddScreenToList( screen );
        EDA_BaseStruct* strct = screen->EEDrawList;
        while( strct )
        {
            if( strct->Type() == DRAW_SHEET_STRUCT_TYPE )
            {
                BuildScreenList( strct );
            }
            strct = strct->Next();
        }
    }
}