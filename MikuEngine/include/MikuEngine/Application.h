#pragma once

namespace MikuEngine
{
	class Application
	{
	public:
		Application();
		~Application();

		void Init();
		void Destroy();

	private:
	};

	Application* EntryPoint();
}
