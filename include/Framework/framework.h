#ifndef __FRAMEWORK_H__
#define __FRAMEWORK_H__

namespace fw
{
	class Framework
	{
	private:
		static Framework* instance;

		bool shouldRun = false;
		volatile int frame = 0;

		static void VBlank();

	protected:
		virtual void Init();
		virtual void Update();
		virtual void LateUpdate();
		virtual void Render();

	public:
		int Run();
		void Stop();

		int GetFrame();
	};
} // namespace fw

#endif