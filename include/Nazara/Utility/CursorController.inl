// Copyright (C) 2017 Jérôme Leclercq
// This file is part of the "Nazara Engine - Utility module"
// For conditions of distribution and use, see copyright notice in Config.hpp

#include <Nazara/Utility/CursorController.hpp>
#include <Nazara/Utility/Debug.hpp>

namespace Nz
{
	inline void CursorController::UpdateCursor(const CursorRef& cursor)
	{
		OnCursorUpdated(this, cursor);
	}
}

#include <Nazara/Utility/DebugOff.hpp>
