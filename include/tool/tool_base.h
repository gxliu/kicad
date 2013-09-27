/*
 * This program source code file is part of KiCad, a free EDA CAD application.
 *
 * Copyright (C) 2013 CERN
 * @author Tomasz Wlostowski <tomasz.wlostowski@cern.ch>
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

#ifndef __TOOL_BASE_H
#define __TOOL_BASE_H

#include <cassert>
#include <base_struct.h>    // for KICAD_T

#include <tool/tool_event.h>
#include <tool/delegate.h>

class EDA_ITEM;
class TOOL_MANAGER;

namespace KiGfx
{
class VIEW;
class VIEW_CONTROLS;
};

enum TOOL_Type
{
    ///> Tool that interacts with the user
    TOOL_Interactive = 0x01,

    ///> Tool that runs in the background without any user intervention
    TOOL_Batch       = 0x02
};

/// Unique identifier for tools
typedef int TOOL_ID;
typedef DELEGATE<int, TOOL_EVENT&> TOOL_STATE_FUNC;

/**
 * Class TOOL_BASE
 *
 * Base abstract interface for all kinds of tools.
 */

class TOOL_BASE 
{
public:
	TOOL_BASE( TOOL_Type aType, TOOL_ID aId, const std::string& aName = std::string( "" ) ) :
		m_type( aType ),
		m_toolId( aId ),
		m_toolName( aName ),
		m_toolMgr( NULL ){};

	virtual ~TOOL_BASE() {};

	/**
	 * Function GetType()
	 * Returns the type of the tool.
	 * @return The type of the tool.
	 */
	TOOL_Type GetType() const
	{
		return m_type;
	}

	/**
	 * Function GetId()
	 * Returns the unique identifier of the tool. The identifier is set by an instance of
	 * TOOL_MANAGER.
	 * @return Identifier of the tool.
	 */
	TOOL_ID GetId() const
	{
		return m_toolId;
	}

	/**
	 * Function GetName()
	 * Returns the name of the tool. Tool names are expected to obey the format:
	 * application.ToolName (eg. pcbnew.InteractiveSelection).
	 * @return The name of the tool.
	 */
	const std::string& GetName() const
	{
		return m_toolName;
	}

	/**
	 * Function GetManager()
	 * Returns the instance of TOOL_MANAGER that takes care of the tool.
	 * @return Instance of the TOOL_MANAGER. If there is no TOOL_MANAGER associated, it returns
	 * NULL.
	 */
	TOOL_MANAGER* GetManager() const
	{
		return m_toolMgr;
	}
	
protected:
	friend class TOOL_MANAGER;

	/**
	 * Function attachManager()
	 *
	 * Sets the TOOL_MANAGER the tool will belong to.
	 * Called by TOOL_MANAGER::RegisterTool()
	 */
	void attachManager( TOOL_MANAGER* aManager );

	/**
	 * Function getView()
	 *
	 * Returns the instance of VIEW object used in the application. It allows tools to draw.
	 * @return The instance of VIEW.
	 */
	KiGfx::VIEW* getView() const;

	/**
	 * Function getViewControls()
	 *
	 * Returns the instance of VIEW_CONTROLS object used in the application. It allows tools to
	 * read & modify user input and its settings (eg. show cursor, enable snapping to grid, etc.)
	 * @return The instance of VIEW_CONTROLS.
	 */
	KiGfx::VIEW_CONTROLS* getViewControls() const;
	
	/**
	 * Function getEditFrame()
	 * 
	 * Returns the application window object, casted to requested user type.
	 */
	template<typename T>
	T* getEditFrame() const
	{
		return static_cast<T*>( getEditFrameInt() );
	}

	/**
	 * Function getModel()
	 * 
	 * Returns the model object if it matches the requested type.
	 */
	template<typename T> 
	T* getModel( KICAD_T modelType ) const
	{
		EDA_ITEM* m = getModelInt();

		return static_cast<T*>( m );
	}

	///> Stores the type of the tool.
	TOOL_Type m_type;

	///> Unique identifier for the tool, assigned by a TOOL_MANAGER instance.
	TOOL_ID m_toolId;

	///> Name of the tool. Names are expected to obey the format application.ToolName
	///> (eg. pcbnew.InteractiveSelection).
	std::string m_toolName;
	TOOL_MANAGER* m_toolMgr;

private:
	// hide the implementation to avoid spreading half of
	// kicad and wxWidgets headers to the tools that may not need them at all!
	EDA_ITEM* getModelInt() const;
	wxWindow* getEditFrameInt() const;
};

#endif
