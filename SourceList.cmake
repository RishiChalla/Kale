set_target_properties(Islands PROPERTIES LINKER_LANGUAGE CXX)

target_sources(Islands PRIVATE

	# -------------- Engine --------------

	src/Engine/EntryPoint.cpp

	src/Engine/Application/Application.hpp
	src/Engine/Application/Application.cpp
	
	src/Engine/Assets/Assets.hpp
	src/Engine/Assets/Assets.cpp
	
	src/Engine/Events/Events.hpp
	src/Engine/Events/Events.cpp
	
	src/Engine/Font/Font.hpp
	src/Engine/Font/Font.cpp
	
	src/Engine/Logger/Logger.hpp
	src/Engine/Logger/Logger.cpp
	
	src/Engine/Math/Vector/Vector.hpp
	
	src/Engine/Scene/Scene.hpp
	src/Engine/Scene/Scene.cpp
	
	src/Engine/Shader/Shader.hpp
	src/Engine/Shader/Shader.cpp
	
	src/Engine/Texture/Texture.hpp
	src/Engine/Texture/Texture.cpp
	
	src/Engine/Window/Window.hpp
	src/Engine/Window/Window.cpp

	# -------------- Game --------------

	src/Game/App/App.hpp
	src/Game/App/App.cpp

	src/Game/Scenes/TitleScene.hpp
	src/Game/Scenes/TitleScene.cpp

	src/Game/Nodes/RoundedRect/RoundedRect.hpp
	src/Game/Nodes/RoundedRect/RoundedRect.cpp
)