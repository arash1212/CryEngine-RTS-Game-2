cmake_minimum_required (VERSION 3.14)
set(CRYENGINE_DIR "D:/CRYENGINE Launcher/Engines/crytek/cryengine-57-lts/5.7.1" CACHE STRING "CRYENGINE root directory.")
set(TOOLS_CMAKE_DIR "${CRYENGINE_DIR}/Tools/CMake")

set(PROJECT_BUILD 1)
set(PROJECT_DIR "${CMAKE_SOURCE_DIR}/../")

include("${TOOLS_CMAKE_DIR}/InitialSetup.cmake")
include("${TOOLS_CMAKE_DIR}/CommonOptions.cmake")

add_subdirectory("${CRYENGINE_DIR}" "${CMAKE_CURRENT_BINARY_DIR}/CRYENGINE")

include("${TOOLS_CMAKE_DIR}/Configure.cmake")
start_sources()

sources_platform(ALL)
add_sources("Code_uber.cpp"
    PROJECTS Game
    SOURCE_GROUP "Root"
		"GamePlugin.cpp"
		"StdAfx.cpp"
		"GamePlugin.h"
		"StdAfx.h"
)
add_sources("Actions_uber.cpp"
    PROJECTS Game
    SOURCE_GROUP "Actions"
		"Actions/IBaseAction.h"
)
add_sources("Units_uber.cpp"
    PROJECTS Game
    SOURCE_GROUP "Actions\\\\Units"
		"Actions/Units/UnitAttackAction.cpp"
		"Actions/Units/UnitMoveAction.cpp"
		"Actions/Units/UnitAttackAction.h"
		"Actions/Units/UnitMoveAction.h"
)
add_sources("Components_uber.cpp"
    PROJECTS Game
    SOURCE_GROUP "Components"
)
add_sources("Controller_uber.cpp"
    PROJECTS Game
    SOURCE_GROUP "Components\\\\Controller"
		"Components/Controller/AIController.cpp"
		"Components/Controller/AIController.h"
)
add_sources("Cover_uber.cpp"
    PROJECTS Game
    SOURCE_GROUP "Components\\\\Cover"
		"Components/Cover/CoverPosition.cpp"
		"Components/Cover/EntityCoverUser.cpp"
		"Components/Cover/CoverPosition.h"
		"Components/Cover/EntityCoverUser.h"
)
add_sources("Managers_uber.cpp"
    PROJECTS Game
    SOURCE_GROUP "Components\\\\Managers"
		"Components/Managers/ActionManager.cpp"
		"Components/Managers/UnitStateManager.cpp"
		"Components/Managers/ActionManager.h"
		"Components/Managers/UnitStateManager.h"
)
add_sources("Player_uber.cpp"
    PROJECTS Game
    SOURCE_GROUP "Components\\\\Player"
		"Components/Player/Player.cpp"
		"Components/Player/PlayerController.cpp"
		"Components/Player/Player.h"
		"Components/Player/PlayerController.h"
)
add_sources("Selectables_uber.cpp"
    PROJECTS Game
    SOURCE_GROUP "Components\\\\Selectables"
		"Components/Selectables/Health.cpp"
		"Components/Selectables/OwnerInfo.cpp"
		"Components/Selectables/Selectable.cpp"
		"Components/Selectables/UIItemProvider.cpp"
		"Components/Selectables/Health.h"
		"Components/Selectables/OwnerInfo.h"
		"Components/Selectables/Selectable.h"
		"Components/Selectables/UIItemProvider.h"
)
add_sources("SpawnPoints_uber.cpp"
    PROJECTS Game
    SOURCE_GROUP "Components\\\\SpawnPoints"
		"Components/SpawnPoints/EnemySpawnPoint.cpp"
		"Components/SpawnPoints/PlayerSpawnPoint.cpp"
		"Components/SpawnPoints/EnemySpawnPoint.h"
		"Components/SpawnPoints/PlayerSpawnPoint.h"
)
add_sources("UI_uber.cpp"
    PROJECTS Game
    SOURCE_GROUP "Components\\\\UI"
		"Components/UI/BoxSelectionUI.cpp"
		"Components/UI/InGameUI.cpp"
		"Components/UI/BoxSelectionUI.h"
		"Components/UI/InGameUI.h"
)
add_sources("Units_uber.cpp"
    PROJECTS Game
    SOURCE_GROUP "Components\\\\Units"
		"Components/Units/Attacker.cpp"
		"Components/Units/BaseAIUnit.cpp"
		"Components/Units/BaseUnit.cpp"
		"Components/Units/UnitAnimation.cpp"
		"Components/Units/Attacker.h"
		"Components/Units/BaseAIUnit.h"
		"Components/Units/BaseUnit.h"
		"Components/Units/UnitAnimation.h"
)
add_sources("Units_uber.cpp"
    PROJECTS Game
    SOURCE_GROUP "Components\\\\Units\\\\Units"
		"Components/Units/Units/Soldier1Unit.cpp"
		"Components/Units/Units/Soldier1Unit.h"
)
add_sources("Weapons_uber.cpp"
    PROJECTS Game
    SOURCE_GROUP "Components\\\\Weapons"
		"Components/Weapons/BaseWeapon.cpp"
		"Components/Weapons/BaseWeapon.h"
)
add_sources("Listeners_uber.cpp"
    PROJECTS Game
    SOURCE_GROUP "Listeners"
		"Listeners/UIElementEventListener.cpp"
		"Listeners/UIElementEventListener.h"
)
add_sources("UIItems_uber.cpp"
    PROJECTS Game
    SOURCE_GROUP "UIItems"
		"UIItems/IBaseUIItem.h"
)
add_sources("Items_uber.cpp"
    PROJECTS Game
    SOURCE_GROUP "UIItems\\\\Items"
)
add_sources("Actionbar_uber.cpp"
    PROJECTS Game
    SOURCE_GROUP "UIItems\\\\Items\\\\Actionbar"
		"UIItems/Items/Actionbar/CancelUIItem.cpp"
		"UIItems/Items/Actionbar/ChangeStanceUIItem.cpp"
		"UIItems/Items/Actionbar/CancelUIItem.h"
		"UIItems/Items/Actionbar/ChangeStanceUIItem.h"
)
add_sources("Utils_uber.cpp"
    PROJECTS Game
    SOURCE_GROUP "Utils"
		"Utils/CoverUtils.cpp"
		"Utils/EntityUtils.cpp"
		"Utils/MathUtils.cpp"
		"Utils/MouseUtils.cpp"
		"Utils/PhysicsUtils.cpp"
		"Utils/CoverUtils.h"
		"Utils/EntityUtils.h"
		"Utils/MathUtils.h"
		"Utils/MouseUtils.h"
		"Utils/PhysicsUtils.h"
)

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/CVarOverrides.h")
    add_sources("NoUberFile"
        PROJECTS Game
        SOURCE_GROUP "Root"
            "CVarOverrides.h"
    )
endif()

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/EngineDefineOverrides.h")
    add_sources("NoUberFile"
        PROJECTS Game
        SOURCE_GROUP "Root"
            "EngineDefineOverrides.h"
    )
endif()
end_sources()

CryGameModule(Game FORCE_SHARED PCH "StdAfx.cpp" SOLUTION_FOLDER "Project")

target_include_directories(${THIS_PROJECT}
PRIVATE
    "${CRYENGINE_DIR}/Code/CryEngine/CryCommon"
    "${CRYENGINE_DIR}/Code/CryEngine/CryAction"
    "${CRYENGINE_DIR}/Code/CryEngine/CrySchematyc/Core/Interface"
    "${CRYENGINE_DIR}/Code/CryPlugins/CryDefaultEntities/Module"
)

if(CMAKE_CXX_COMPILER_ID MATCHES "[Cc]lang")
    target_compile_options(${THIS_PROJECT} PRIVATE
        -Wno-unused-variable
        -Wno-reorder
        -Wno-unknown-pragmas
        -Wno-parentheses
        -Wno-switch
        -Wno-format
        -Wno-dynamic-class-memaccess
        -Wno-unused-private-field
        -Wno-unused-value
        -Wno-invalid-offsetof
        -Wno-multichar
        -Wno-char-subscripts
        -Wno-null-conversion
        -Wno-empty-body
        -Wno-unused-lambda-capture
        -Wno-unused-function
        -Wno-tautological-constant-out-of-range-compare
    )
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(${THIS_PROJECT} PRIVATE
        -Wno-unused-function
        -Wno-unused-value
        -Wno-unused-variable
        -Wno-aligned-new
        -Wno-int-in-bool-context
        -Wno-invalid-offsetof
        -Wno-comment
        -Wno-sign-compare
        -Wno-unused-but-set-variable
        -Wno-maybe-uninitialized
        -Wno-misleading-indentation
        -Wno-unused-result
        -Wno-unknown-pragmas
        -Wno-multichar
        -Wno-strict-aliasing
        -Wno-char-subscripts
        -Wno-conversion-null
        -Wno-reorder
        -Wno-parentheses
        -Wno-format
        -Wno-switch
    )
endif()


if(OPTION_ENGINE)
    if(NOT EXISTS "${CRYENGINE_DIR}/Code/Sandbox/EditorQt")
        add_library(Editor STATIC "${CRYENGINE_DIR}/Code/CryEngine/CryCommon/CryCore/Platform/platform.h")
        set_target_properties(Editor PROPERTIES LINKER_LANGUAGE CXX)
        if (WIN32)
            set_visual_studio_debugger_command(Editor "${CRYENGINE_DIR}/bin/win_x64/Sandbox.exe" "-project \"${PROJECT_DIR}/Game.cryproject\"")
        endif()
    endif()
else()
    add_library(GameLauncher STATIC "${CRYENGINE_DIR}/Code/CryEngine/CryCommon/CryCore/Platform/platform.h")
    set_target_properties(GameLauncher PROPERTIES LINKER_LANGUAGE CXX)
    if (WIN32)
        set_visual_studio_debugger_command(GameLauncher "${CRYENGINE_DIR}/bin/win_x64/GameLauncher.exe" "-project \"${PROJECT_DIR}/Game.cryproject\"")
    endif()

    add_library(Editor STATIC "${CRYENGINE_DIR}/Code/CryEngine/CryCommon/CryCore/Platform/platform.h")
    set_target_properties(Editor PROPERTIES LINKER_LANGUAGE CXX)
    if (WIN32)
        set_visual_studio_debugger_command(Editor "${CRYENGINE_DIR}/bin/win_x64/Sandbox.exe" "-project \"${PROJECT_DIR}/Game.cryproject\"")
    endif()

    add_library(GameServer STATIC "${CRYENGINE_DIR}/Code/CryEngine/CryCommon/CryCore/Platform/platform.h")
    set_target_properties(GameServer PROPERTIES LINKER_LANGUAGE CXX)
    if (WIN32)
        set_visual_studio_debugger_command(GameServer "${CRYENGINE_DIR}/bin/win_x64/Game_Server.exe" "-project \"${PROJECT_DIR}/Game.cryproject\"")
    endif()
endif()

# Set StartUp project in Visual Studio
set_solution_startup_target(GameLauncher)

if (WIN32)
    set_visual_studio_debugger_command( ${THIS_PROJECT} "${CRYENGINE_DIR}/bin/win_x64/GameLauncher.exe" "-project \"${PROJECT_DIR}/Game.cryproject\"" )
endif()

#BEGIN-CUSTOM
# Make any custom changes here, modifications outside of the block will be discarded on regeneration.
#END-CUSTOM