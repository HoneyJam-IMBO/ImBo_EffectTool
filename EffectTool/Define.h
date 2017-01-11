#pragma once

//texture, constant buffer bind flag
#define BIND_VS 0b000001   // 0x0001 //0001
#define BIND_HS 0b000010   // 0x0002 //0010
#define BIND_DS 0b000100   // 0x0004 //0100
#define BIND_GS 0b001000   // 0x0008 //1000
#define BIND_PS 0b010000   // 0x0010 //...
#define BIND_CS 0b100000   // 0x0020 //..


#define NO_COPY(CLASSNAME)							\
	private:										\
		CLASSNAME(const CLASSNAME&);				\
		CLASSNAME& operator =(const CLASSNAME&);	\


#define DECLARE_SINGLETON(CLASSNAME)				\
		NO_COPY(CLASSNAME)							\
		private:									\
			static CLASSNAME*			pInstance;	\
		public:										\
			static CLASSNAME*	GetInstance(void);	\
			static void DestroyInstance(void);		\

#define IMPLEMENT_SINGLETON(CLASSNAME)				\
	CLASSNAME* CLASSNAME::pInstance = NULL;			\
	CLASSNAME* CLASSNAME::GetInstance(void){		\
	if(pInstance == NULL){							\
		pInstance = new CLASSNAME;}					\
		return pInstance;  }						\
		void CLASSNAME::DestroyInstance(void){		\
		if(pInstance != NULL){						\
			delete pInstance;						\
			pInstance = NULL;}						\
			}


#define ERR_MSG(Message)				MessageBox(g_hWnd, Message, L"System Error", NULL)
template<typename Ty, size_t N>
constexpr size_t GetArraySize(Ty(&)[N]) noexcept
{
	return N;
}

extern HWND	g_hWnd;
