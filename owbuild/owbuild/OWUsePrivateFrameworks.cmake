# - ow_use_private_frameworks(framework1 ... frameworkN)
# MacOSX only, uses privately a framework inside the current project:
# imports properties from a framework to the current project
#
# Copyright (C) 2006-2007  Wengo
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING file.


if (APPLE)
	include(CMakeFindFrameworks)

	macro (ow_use_private_frameworks)

		ow_check_project()

		foreach (fwk ${ARGN})
			cmake_find_frameworks(${fwk})

			set(first ${${fwk}_FRAMEWORKS})

			ow_add_private_link_flags(
				"-framework ${fwk}"
			)
		endforeach (fwk ${ARGN})

	endmacro (ow_use_private_frameworks)
endif (APPLE)
