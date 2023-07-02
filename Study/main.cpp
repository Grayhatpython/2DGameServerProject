#include "pch.h"
#include <iostream>
#include "BoundedQueue.h"
#include "ConcurrentQueue.h"
#include <set>

using namespace std;

#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

int main()
{
	std::cout << "테스트";
	return 0;
}

/*
#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

using namespace std;

bool init();
void kill();
bool loop();

// Pointers to our window, renderer, texture, and font
SDL_Window* window;
SDL_Renderer* renderer;
SDL_Texture* texture, * text, * cursor;
TTF_Font* font;
wstring input;

uint64 startTick = ::GetTickCount64() + 500;
uint64 startTick2 = ::GetTickCount64() + 5000;
bool toggle = true;
WCHAR cursorText[1];


int main(int argc, char** args)
{


	std::wstring t = L"rwerew\n";

	auto fount = t.find(L'\n');
	auto last = t.length() - 1;

	auto s = t[fount];

	setlocale(LC_ALL, "");
	//input.resize(256);
	cursorText[0] = L'|';

	if (!init()) {
		system("pause");
		return 1;
	}

	while (loop()) {
		// wait before processing the next frame
		//SDL_Delay(10);
	}
	keybd_event(VK_HANGEUL, 0, KEYEVENTF_KEYUP, 0);// 누름 해제

	while (loop()) {
		// wait before processing the next frame
		//SDL_Delay(10);
	}

	kill();
	return 0;
}

bool test = false;
bool loop() {


	static const unsigned char* keys = SDL_GetKeyboardState(NULL);

	SDL_Event e;
	SDL_Rect dest;

	// Clear the window to white
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderClear(renderer);

	if (startTick < ::GetTickCount64())
	{
		if (toggle)
		{
			cursorText[0] = L'|';
			toggle = false;
		}
		else
		{
			cursorText[0] = L' ';
			toggle = true;
		}

		startTick = ::GetTickCount64() + 500;
	}
	
	if (test == false && startTick2 < ::GetTickCount64())
	{
		test = true;
		return false;
	}

	// Event loop
	while (SDL_PollEvent(&e) != 0) {
		switch (e.type) {
		case SDL_QUIT:
			return false;
		case SDL_TEXTINPUT:
		{
			auto t = UtilityHelper::ConvertUTF8ToUnicode(e.text.text);
			input += t;
		}
		break;
		case SDL_KEYDOWN:
			if (e.key.keysym.sym == SDLK_BACKSPACE && input.size()) {
				input.pop_back();
			}
			break;
		}
	}



	// Render texture
	SDL_RenderCopy(renderer, texture, NULL, NULL);

	SDL_Color foreground = { 0, 0, 0 };

	{
		int32 offset = 0;
		if (input.size())
		{
			SDL_Surface* text_surf = TTF_RenderUNICODE_Blended(font, (const uint16*)input.c_str(), foreground);
			text = SDL_CreateTextureFromSurface(renderer, text_surf);

			dest.x = 320 - (text_surf->w / 2.0f);
			dest.y = 240;
			dest.w = text_surf->w;
			dest.h = text_surf->h;
			offset = (text_surf->w / 2.0f) - 5;

			SDL_RenderCopy(renderer, text, NULL, &dest);
			SDL_DestroyTexture(text);
			SDL_FreeSurface(text_surf);
		}

		SDL_Surface* cursor_surf = TTF_RenderUNICODE_Blended(font, (const uint16*)cursorText, foreground);
		cursor = SDL_CreateTextureFromSurface(renderer, cursor_surf);

		dest.x = 320 + offset;
		dest.y = 240;
		dest.w = cursor_surf->w;
		dest.h = cursor_surf->h;

		SDL_RenderCopy(renderer, cursor, NULL, &dest);
		SDL_DestroyTexture(cursor);
		SDL_FreeSurface(cursor_surf);
	}

	// Update window
	SDL_RenderPresent(renderer);

	return true;
}

bool init() {
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		cout << "Error initializing SDL: " << SDL_GetError() << endl;
		return false;
	}

	if (IMG_Init(IMG_INIT_PNG) < 0) {
		cout << "Error initializing SDL_image: " << IMG_GetError() << endl;
		return false;
	}

	// Initialize SDL_ttf
	if (TTF_Init() < 0) {
		cout << "Error intializing SDL_ttf: " << TTF_GetError() << endl;
		return false;
	}

	window = SDL_CreateWindow("Example", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN);
	if (!window) {
		cout << "Error creating window: " << SDL_GetError() << endl;
		return false;
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (!renderer) {
		cout << "Error creating renderer: " << SDL_GetError() << endl;
		return false;
	}

	//SDL_Surface* buffer = IMG_Load("test.png");
	//if (!buffer) {
	//	cout << "Error loading image test.png: " << SDL_GetError() << endl;
	//	return false;
	//}

	//texture = SDL_CreateTextureFromSurface(renderer, buffer);
	//SDL_FreeSurface(buffer);
	//buffer = NULL;
	//if (!texture) {
	//	cout << "Error creating texture: " << SDL_GetError() << endl;
	//	return false;
	//}

	// Load font
	font = TTF_OpenFont("font.ttf", 16);
	if (!font) {
		cout << "Error loading font: " << TTF_GetError() << endl;
		return false;
	}

	// Start sending SDL_TextInput events
	//SDL_StartTextInput();

	return true;
}

void kill() {
	SDL_StopTextInput();

	TTF_CloseFont(font);
	SDL_DestroyTexture(texture);
	texture = NULL;

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	window = NULL;
	renderer = NULL;

	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}



class Test
{
	int a = 0;
};

int main()
{

	set<std::shared_ptr<Test>> t;
	auto tt = std::make_shared<Test>();
	t.insert(tt);

	auto sessions = t;

	int a = 3;
	return 0;
}


class Test
{
public:
	void l()
	{
		WRITE_LOCK;
	}

public:
	USE_LOCK;
};
class Test2
{

};
int main()
{
	alignas(64) std::vector<Test> ssdf;
	alignas(64) size_t ddd;
	std::cout << sizeof(ddd) << std::endl;
	srand(time(NULL));
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	BoundedQueue<int> q(1024);

	std::vector<std::thread> ts;
	for (int32 i = 0; i < 12; i++)
	{
		ts.push_back(std::thread([&, i]() {

			while (true)
			{
				q.Push(std::move(i + 1));
				std::this_thread::sleep_for(5ms);
			}

			}));
	}

	std::vector<std::thread> tss;
	for (int32 i = 0; i < 1; i++)
	{
		tss.push_back(std::thread([&]() {

			while (true)
			{
				int a = 0;
				q.Pop(a);
				
				std::cout << a << std::endl;
			}

			}));
	}


	for (auto& t : ts)
		t.join();


	for (auto& t : tss)
		t.join();

	return 0;
}


struct Tile
{
	std::vector<Tile*>	adjacent;
	Tile*				parent = nullptr;

	float				f = 0.f;
	float				g = 0.f;

	bool				isVisited = false;
	bool				isBlocked = false;
};

class Performance
{
public:
	Performance()
	{
		QueryPerformanceFrequency(&timer);
		QueryPerformanceCounter(&start);
	}
	~Performance()
	{
		QueryPerformanceCounter(&end);

		DeltaTime = (end.QuadPart - start.QuadPart) / (float)timer.QuadPart * 1000;
		auto t = std::to_string(DeltaTime) + "\n";
		::OutputDebugString(t.c_str());
	}

private:
	LARGE_INTEGER timer{}, start{}, end{};
	float DeltaTime = 0.f;
};

class Zone
{

};

int main()
{	
	std::vector<std::vector<std::unique_ptr<Zone>>>	_zones;
	_zones.resize(10);

	for (int32 row = 0; row < 10; row++)
	{
		for (int32 column = 0; column < 10; column++)
			_zones[row].push_back(std::make_unique<Zone>());
	}

	int32 rows = 200;
	int32 columns = 200;

	std::vector<std::vector<Tile*>>	 tiles;
	std::vector<std::vector<Tile*>>	 tiles2;
	tiles.resize(rows, std::vector<Tile*>(columns, nullptr));
	tiles2.resize(rows, std::vector<Tile*>(columns, nullptr));

	for (int32 row = 0; row < rows; row++)
	{
		for (int32 column = 0; column < columns; column++)
		{
			auto tile = new Tile();
			tiles[row][column] = tile;
		}
	}

	for (int32 row = 0; row < rows; row++)
	{
		for (int32 column = 0; column < columns; column++)
		{
			auto tile = new Tile();
			tiles2[row][column] = tile;
		}
	}

	{
		Performance p;
		auto start = ::GetTickCount64();
		auto end = ::GetTickCount64();
		::OutputDebugString(std::to_string(end - start).c_str());
		::OutputDebugString("\n");
	}

	for (auto& rows : tiles)
	{
		for (auto tile : rows)
			delete tile;
	}

	for (auto& rows : tiles2)
	{
		for (auto tile : rows)
			delete tile;
	}
	return 0;

}



class Test
{
public:
	const std::unordered_map<int32, Test>& Get() { return m; }
	std::unordered_map<int32, Test> Get2() { return m; }
public:
	int a = 3;

	std::unordered_map<int32, Test> m;
};


int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	Test tt;
	Test t1;
	t1.a = 5;
	Test t2;
	t2.a = 1;
	Test t3;
	t3.a = 2;

	tt.m.insert(std::make_pair(1, t1));
	tt.m.insert(std::make_pair(2, t2));
	tt.m.insert(std::make_pair(3, t3));

	const auto& aa = tt.Get();
	std::map<int32, Test> mm;
	for (auto& ac : aa)
		mm.insert(std::make_pair(ac.second.a, ac.second));

	int a = 3;


	//BoundedQueue<std::shared_ptr<Test>> q(65536);
	//
	//std::vector<std::thread> pushThreads;

	//for (int32 i = 0; i < 12; i++)
	//{
	//	pushThreads.push_back(std::thread([&q, i]() {
	//		while (true)
	//		{
	//			auto test = std::make_shared<Test>();
	//			test->a = i;
	//			q.Push(std::move(test));
	//			std::this_thread::sleep_for(1ms);
	//		}
	//		}));
	//}

	//std::this_thread::sleep_for(50ms);

	//std::thread popThread([&q]() {
	//		while (true)
	//		{
	//			std::shared_ptr<Test> test;
	//			q.Clear();
	//			//if(test)
	//				//std::cout << test->a << std::endl;

	//			if (q.Empty())
	//				std::cout << "걸릿다!" << std::endl;
	//		}
	//	});
	//
	//for (auto& pushThread : pushThreads)
	//	pushThread.join();

	//popThread.join();

	return 0;
}



template <typename T>
class BoundedThreadsafeQueue
{
private:
	struct Node
	{
		std::shared_ptr<T> data;
		std::unique_ptr<Node> next;
	};

	std::mutex head_mutex_;
	std::unique_ptr<Node> head_;
	std::mutex tail_mutex_;
	Node* tail_;
	std::condition_variable data_cond;
	uint32_t max_size_;
	std::atomic<uint32_t> size_{ 0 };

private:
	Node* GetTail();
	std::unique_ptr<Node> PopHead();
	std::unique_ptr<Node> TryPopHead();
	std::unique_lock<std::mutex> WaitForData();
	std::unique_ptr<Node> WaitPopHead();

public:
	BoundedThreadsafeQueue() : head_(new Node), tail_(head_.get())
	{
		max_size_ = -1;
	}
	explicit BoundedThreadsafeQueue(uint32_t max_size) : head_(new Node), tail_(head_.get())
	{
		max_size_ = max_size;
	}
	BoundedThreadsafeQueue(const BoundedThreadsafeQueue& other) = delete;

	BoundedThreadsafeQueue& operator=(const BoundedThreadsafeQueue& other) = delete;

	std::shared_ptr<T> TryPop();
	bool TryPop(T& value);
	std::shared_ptr<T> WaitAndPop();
	void WaitAndPop(T& value);
	void Push(T new_value);
	bool Empty();
	uint32_t GetSize();
};

template <typename T>
typename BoundedThreadsafeQueue<T>::Node* BoundedThreadsafeQueue<T>::GetTail()
{
	std::lock_guard<std::mutex> tail_lock{ tail_mutex_ };
	return tail_;
}

template <typename T>
std::unique_ptr<typename BoundedThreadsafeQueue<T>::Node> BoundedThreadsafeQueue<T>::PopHead()
{
	std::unique_ptr<Node> old_head = std::move(head_);
	head_ = std::move(old_head->next);
	return old_head;
}

template <typename T>
std::unique_ptr<typename BoundedThreadsafeQueue<T>::Node> BoundedThreadsafeQueue<T>::TryPopHead()
{
	std::lock_guard<std::mutex> head_lock(head_mutex_);
	if (head_.get() == GetTail())
	{
		return std::unique_ptr<Node>();
	}
	size_.fetch_sub(1);
	return PopHead();
}

template <typename T>
std::unique_lock<std::mutex> BoundedThreadsafeQueue<T>::WaitForData()
{
	std::unique_lock<std::mutex> head_lock(head_mutex_);
	data_cond.wait(head_lock, [&]
		{ return head_.get() != GetTail(); });
	return std::move(head_lock);
}

template <typename T>
std::unique_ptr<typename BoundedThreadsafeQueue<T>::Node> BoundedThreadsafeQueue<T>::WaitPopHead()
{
	std::unique_lock<std::mutex> head_lock(WaitForData());
	return PopHead();
}

template <typename T>
std::shared_ptr<T> BoundedThreadsafeQueue<T>::WaitAndPop()
{
	std::unique_ptr<Node> const old_head = WaitPopHead();
	return old_head->data;
}

template <typename T>
bool BoundedThreadsafeQueue<T>::TryPop(T& value)
{
	std::unique_ptr<Node> old_head = TryPopHead();
	if (old_head)
	{
		value = std::move(*old_head->data);
		return true;
	}
	return false;
}

template <typename T>
std::shared_ptr<T> BoundedThreadsafeQueue<T>::TryPop()
{
	std::unique_ptr<Node> old_head = TryPopHead();
	return old_head ? old_head->data : std::shared_ptr<T>();
}

template <typename T>
void BoundedThreadsafeQueue<T>::Push(T new_value)
{
	if (size_.load(std::memory_order_relaxed) == max_size_)
	{
		throw std::length_error("queue size exceeded limit!");
	}
	std::shared_ptr<T> new_data(
		std::make_shared<T>(std::move(new_value)));
	std::unique_ptr<Node> p(new Node);
	Node* const new_tail = p.get();
	std::lock_guard<std::mutex> tail_lock(tail_mutex_);
	tail_->data = new_data;
	tail_->next = std::move(p);
	tail_ = new_tail;
	size_.fetch_add(1);
}

template <typename T>
bool BoundedThreadsafeQueue<T>::Empty()
{
	std::lock_guard<std::mutex> lk{ head_mutex_ };
	return head_.get() == GetTail();
}

template <typename T>
uint32_t BoundedThreadsafeQueue<T>::GetSize()
{
	return size_.load(std::memory_order_relaxed);
}

class Test
{
public:
	void l()
	{
		WRITE_LOCK;
	}

public:
	USE_LOCK;
};
class Test2
{

};
int main()
{
	alignas(64) std::vector<Test> ssdf;
	alignas(64) size_t ddd;
	std::cout << sizeof(ddd) << std::endl;
	srand(time(NULL));
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	BoundedQueue<int> q(16777216);
	BoundedThreadsafeQueue<int> qq(1048576);
	ConcurrentQueue<int> qqq;

	for (int32 i = 0; i < 10000000; i++)
	{
		q.Push(std::move(i));
	}

	//for (int32 i = 0; i < 1048576; i++)
	//{
	//	qq.Push(i);
	//}


	/*
	std::thread sss([&qqq]() {

		LThreadId = 1;
		while (true)
		{
			for (int32 i = 0; i < 10000000; i++)
			{
				qqq.Push(i);
			}
			break;
		}

	});

	sss.join();
	
	

	std::vector<std::thread> ts;
	for (int32 i = 0; i < 10; i++)
	{
		ts.push_back(std::thread([&]() {

			while (true)
			{
				int a;
				if (q.Pop(a) == false)
					break;
			}

			}));
	}

	uint64 start = ::GetTickCount64();

	for (auto& t : ts)
		t.join();

	uint64 end = ::GetTickCount64();
	std::cout << end - start << std::endl;


	std::vector<std::thread> tss;
	for (int32 i = 0; i < 2; i++)
	{
		tss.push_back(std::thread([&]() {

			while (true)
			{
				if (qq.TryPop() == nullptr)
					break;
			}

			}));
	}

	uint64 start2 = ::GetTickCount64();

	for (auto& t : tss)
		t.join();

	uint64 end2 = ::GetTickCount64();
	std::cout << end2 - start2 << std::endl;


	int a = 0;
	Test test;
	std::vector<std::thread> tsss;
	for (int32 i = 2; i <= 12; i++)
	{
		tsss.push_back(std::thread([&qqq, i, &a, &test]() {

				LThreadId = i;
				while (true)
				{
					//a++;
					if (qqq.Pop() == -1)
						break;
					//test.l();
				}

				//for (int32 i = 0; i < 1048576; i++)
				//{
					//test.l();
				//}

			}));
	}

	uint64 start3 = ::GetTickCount64();

	for (auto& t : tsss)
		t.join();

	uint64 end3 = ::GetTickCount64();
	std::cout << end3 - start3 << std::endl;
	std::cout << a << std::endl;
		
	return 0;
}









template<typename T>
class CallableBase;

template<typename Ret, typename... Args>
class CallableBase<Ret(Args...)>
{
public:
	virtual ~CallableBase() = default;
	virtual Ret Invoke(Args... args) abstract;
};

template<typename T, typename Type, typename MemberFunc>
class MemberFuncCallable;

//	TODO
template<typename Ret, typename... Args, typename Type, typename MemberFunc>
class MemberFuncCallable<Ret(Args...), Type, MemberFunc> : public CallableBase<Ret(Args...)>
{
public:
	MemberFuncCallable(Type* owner, MemberFunc memberFunc)
		: _owner(owner), _memberFunc(memberFunc)
	{

	}

public:
	virtual Ret Invoke(Args... args) override
	{
		return  (_owner->*_memberFunc)(std::forward<Args>(args)...);
	}

private:
	Type* _owner;
	MemberFunc	_memberFunc;
};

template<typename T>
class EventHandler;

template<typename T>
class Delegate;

template<typename Ret, typename... Args>
class Delegate<Ret(Args...)>
{
	friend class EventHandler<Ret(Args...)>;

public:
	Delegate() = default;
	Delegate(Delegate&& other) noexcept;
	Delegate& operator=(Delegate&& other) noexcept;
	~Delegate();

public:
	template<typename T, typename MemberFunc>
	void Bind(T* owner, MemberFunc memberFunc);

	Ret operator()(Args... args) const;
	Ret Invoke(Args... args) const;

	//private:
	//	Delegate(const Delegate& other) = delete;
	//	Delegate& operator=(Delegate const& other) = delete;

private:
	CallableBase<Ret(Args...)>* _callable = nullptr;
};

template<typename Ret, typename... Args>
Delegate<Ret(Args...)>::Delegate(Delegate&& other) noexcept
	: _callable(other._callable)
{
	other._callable = nullptr;
}

template<typename Ret, typename... Args>
Delegate<Ret(Args...)>& Delegate<Ret(Args...)>::operator=(Delegate&& other) noexcept
{
	Delegate temp(std::move(other));
	return *this;
}

template<typename Ret, typename... Args>
Delegate<Ret(Args...)>::~Delegate()
{
	delete _callable; 
	_callable = nullptr;
}

template<typename Ret, typename... Args>
template<typename T, typename MemberFunc>
void Delegate<Ret(Args...)>::Bind(T* owner, MemberFunc memberFunc)
{
	_callable = new MemberFuncCallable<Ret(Args...), T, MemberFunc>(owner, memberFunc);
}

template<typename Ret, typename... Args>
Ret Delegate<Ret(Args...)>::operator()(Args... args) const
{
	return _callable->Invoke(std::forward<Args>(args)...);
}

template<typename Ret, typename... Args>
Ret  Delegate<Ret(Args...)>::Invoke(Args... args) const
{
	return _callable->Invoke(std::forward<Args>(args)...);
}


template<typename T>
class EventHandler;

template<typename T>
class CallableBase;

class ConnectedEventHandler
{
public:
	template<typename Ret, typename... Args>
	ConnectedEventHandler(EventHandler<Ret(Args...)>* eventHandler, CallableBase<Ret(Args...)>* callable)
		: _eventHandler(eventHandler), _callable(callable), _disconnectFunc(&DisconnectFunc<Ret, Args...>)
	{

	}

	void Disconnect()
	{
		if (_disconnectFunc)
			_disconnectFunc(_eventHandler, _callable);
	}

private:
	template<typename Ret, typename... Args>
	static void DisconnectFunc(void* signal, void* callable)
	{
		static_cast<EventHandler<Ret(Args...)>*>(signal)->UnBind(static_cast<CallableBase<Ret(Args...)>*>(callable));
	}

private:
	std::function<void(void*, void*)>	_disconnectFunc;
	void*								_eventHandler = nullptr;
	void*								_callable = nullptr;
};



template<typename T>
class EventHandler;

template<typename Ret, typename... Args>
class EventHandler<Ret(Args...)>
{
	friend class ConnectedEventHandler;

public:
	template<typename T, typename MemberFunc>
	ConnectedEventHandler Bind(T* owner, MemberFunc memberFunc);

	void operator()(Args... args);
	void Invoke(Args... args);

private:
	void UnBind(CallableBase<Ret(Args...)>* callable);

private:
	std::vector<Delegate<Ret(Args...)>> _delegates;
};

template<typename Ret, typename... Args>
template<typename T, typename MemberFunc>
ConnectedEventHandler EventHandler<Ret(Args...)>::Bind(T* owner, MemberFunc memberFunc)
{
	Delegate<Ret(Args...)> delegate;
	delegate.Bind(owner, memberFunc);
	CallableBase<Ret(Args...)>* callable = delegate._callable;
	_delegates.push_back(std::move(delegate));
	return ConnectedEventHandler(this, callable);
}

template<typename Ret, typename... Args>
void EventHandler<Ret(Args...)>::operator()(Args... args)
{
	Invoke(std::forward<Args>(args)...);
}

template<typename Ret, typename... Args>
void EventHandler<Ret(Args...)>::Invoke(Args... args)
{
	for (auto& delegate : _delegates)
		delegate.Invoke(std::forward<Args>(args)...);
}

template<typename Ret, typename... Args>
void EventHandler<Ret(Args...)>::UnBind(CallableBase<Ret(Args...)>* callable)
{
	for (auto it = _delegates.begin(), end = _delegates.end(); it != end; ++it)
	{
		if (it->_callable == callable)
		{
			_delegates.erase(it);
			return;
		}
	}
}

#define EVENT_TYPE(NAME, ...)			using NAME = EventHandler<void(__VA_ARGS__)>
#define EVENT_TYPE_RET(NAME,RET, ...)	using NAME = EventHandler<RET(__VA_ARGS__)>

class InputManager
{
public:
	template<typename T, typename MemberFunc>
	ConnectedEventHandler AddKeyPressEventListener(T* owner, MemberFunc memberFunc)
	{
		return _keyPressEvent.Bind(owner, memberFunc);
	}
	EVENT_TYPE(keyBoardEvent, float);
	keyBoardEvent _keyPressEvent;

	void Condition()
	{
		_keyPressEvent(3.f);
	}
};

InputManager GInputManager;

class GUI
{
public:
	virtual void OnKeyPressed(float x)
	{

	}
};

class GUIManager
{
public:
	GUIManager()
	{
		_f.push_back(GInputManager.AddKeyPressEventListener(this, &GUIManager::OnKeyPressed));
	}
	~GUIManager()
	{
		for (auto f : _f)
			f.Disconnect();

		for (auto gui : _guis)
			delete gui;
	}

	void OnKeyPressed(float x)
	{
		
		//	엑티브된 유아이 1번이라 가정하고 실행되어야 한다면
		_guis[1]->OnKeyPressed(x);
	}

	std::vector<GUI*> _guis;

	std::vector<ConnectedEventHandler> _f;
};

class Item : public GUI
{

};

class Inven : public GUI
{
public:
	virtual void OnKeyPressed(float x) override
	{
		std::cout << "와우 : " << x << std::endl;
	}
};

#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>


int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	GUIManager manager;
	manager._guis.push_back(new Item());
	manager._guis.push_back(new Inven());

	GInputManager.Condition();

	return 0;
}



struct Stat
{
	Stat()
	{
		t = new int32();
	}

	~Stat()
	{
		delete t;
	}

	int32 level = 0;
	int32 maxHp = 0;
	int32 attack = 0;
	int32 totalExp = 0;
	int32* t = nullptr;
};

struct Skill
{
	int32			id = 0;
	std::wstring	name;
	float			coolDown = 0.f;
	int32			damage = 0;
	std::wstring	skillType;
};

struct ServerConfig
{
	std::wstring dataPath;
};

class ConfigManager
{
public:
	static bool	LoadConfig();

public:
	static ServerConfig serverConfig;
};

ServerConfig ConfigManager::serverConfig;

class DataManager
{
public:
	static std::unordered_map<int32, Stat> _stats;
};

std::unordered_map<int32, Stat> DataManager::_stats;

#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

int main()
{
	std::wcout.imbue(std::locale("korean"));
	Stat stat;

	//DataManager::_stats.insert(std::make_pair(1, stat));
	//ConfigManager::serverConfig.dataPath = L"test";
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	
	rapidjson::Document doc;
	JsonParser::ParseFromFile("config.json", doc);

	const rapidjson::Value& statsValue = doc["dataPath"];
	auto str = UtilityHelper::ConvertUTF8ToUnicode(statsValue.GetString());

	std::wcout << str << std::endl;

	return 0;
}


class Parent;
class Test
{
public:
	~Test()
	{

	}
public:
	std::shared_ptr<Parent> _t;
};


class Child
{

public:
	std::shared_ptr<Parent> _p;
};

class Parent : public std::enable_shared_from_this<Parent>
{
public:
	virtual ~Parent()
	{

	}

	Child _c;
};

class Parent2 : public Parent
{
public:
	virtual ~Parent2()
	{

	}
};


int main()
{
	Test t;
	auto p = std::make_shared<Parent2>();
	p->_c._p = p;

	t._t = p;

	p->_c._p = nullptr;
	return 0;
}

class State
{
public:
	State()
	{

	}
	virtual ~State()
	{

	}
};

class Idle : public State
{
public:
	Idle()
	{

	}
	virtual ~Idle()
	{

	}
};

class Test
{
public:
	Test()
	{

	}
	~Test()
	{

	}

	template<typename T>
	T* AddState()
	{
		std::unique_ptr<State> state = std::make_unique<T>();
		_states[L"Idle"] = std::move(state);
		return static_cast<T*>(_states[L"Idle"].get());
	}

	std::unordered_map<std::wstring, std::unique_ptr<State>> _states;
};


int main()
{
	Test a;
	a.AddState<Idle>();

	return 0;
}


#define TO_STRING( x ) #x

//****************
// CLASS_DECLARATION
//
// This macro must be included in the declaration of any subclass of Component.
// It declares variables used in type checking.
//****************
#define CLASS_DECLARATION( classname )                                                      \
public:                                                                                     \
    static const std::size_t Type;                                                          \
    virtual bool IsClassType( const std::size_t classType ) const override;                 \

//****************
// CLASS_DEFINITION
// 
// This macro must be included in the class definition to properly initialize 
// variables used in type checking. Take special care to ensure that the 
// proper parentclass is indicated or the run-time type information will be
// incorrect. Only works on single-inheritance RTTI.
//****************
#define CLASS_DEFINITION( parentclass, childclass )                                         \
const std::size_t childclass::Type = std::hash< std::string >()( TO_STRING( childclass ) ); \
bool childclass::IsClassType( const std::size_t classType ) const {                         \
        if ( classType == childclass::Type )                                                \
            return true;                                                                    \
        return parentclass::IsClassType( classType );                                       \
}                                                                                           \

namespace rtti {

	//***************
	// Component
	// base class
	//***************
	class Component {
	public:

		static const std::size_t                    Type;
		virtual bool                                IsClassType(const std::size_t classType) const {
			return classType == Type;
		}

	public:

		virtual                                ~Component() = default;
		Component(std::string&& initialValue)
			: value(initialValue) {
		}

	public:

		std::string                             value = "uninitialized";
	};

	//***************
	// Collider
	//***************
	class Collider : public Component {

		CLASS_DECLARATION(Collider)

	public:

		Collider(std::string&& initialValue)
			: Component(std::move(initialValue)) {
		}
	};

	//***************
	// BoxCollider
	//***************
	class BoxCollider : public Collider {

		CLASS_DECLARATION(BoxCollider)

	public:

		BoxCollider(std::string&& initialValue)
			: Collider(std::move(initialValue)) {
		}
	};

	//***************
	// RenderImage
	//***************
	class RenderImage : public Component {

		CLASS_DECLARATION(RenderImage)

	public:

		RenderImage(std::string&& initialValue)
			: Component(std::move(initialValue)) {
		}
	};

	//***************
	// GameObject
	//***************
	class GameObject {
	public:

		std::vector< std::unique_ptr< Component > > components;

	public:

		template< class ComponentType, typename... Args >
		void                                    AddComponent(Args&&... params);

		template< class ComponentType >
		ComponentType& GetComponent();

		template< class ComponentType >
		bool                                    RemoveComponent();

		template< class ComponentType >
		std::vector< ComponentType* >          GetComponents();

		template< class ComponentType >
		int                                     RemoveComponents();
	};

	//***************
	// GameObject::AddComponent
	// perfect-forwards all params to the ComponentType constructor with the matching parameter list
	// DEBUG: be sure to compare the arguments of this fn to the desired constructor to avoid perfect-forwarding failure cases
	// EG: deduced initializer lists, decl-only static const int members, 0|NULL instead of nullptr, overloaded fn names, and bitfields
	//***************
	template< class ComponentType, typename... Args >
	void GameObject::AddComponent(Args&&... params) {

		components.emplace_back(std::make_unique< ComponentType >(std::forward< Args >(params)...));
	}

	//***************
	// GameObject::GetComponent
	// returns the first component that matches the template type
	// or that is derived from the template type
	// EG: if the template type is Component, and components[0] type is BoxCollider
	// then components[0] will be returned because it derives from Component
	//***************
	template< class ComponentType >
	ComponentType& GameObject::GetComponent() {
		for (auto&& component : components) {
			if (component->IsClassType(ComponentType::Type))
				return *static_cast<ComponentType*>(component.get());
		}

		return *std::unique_ptr< ComponentType >(nullptr);
	}

	//***************
	// GameObject::RemoveComponent
	// returns true on successful removal
	// returns false if components is empty, or no such component exists
	//***************
	template< class ComponentType >
	bool GameObject::RemoveComponent() {
		if (components.empty())
			return false;

		auto& index = std::find_if(components.begin(),
			components.end(),
			[classType = ComponentType::Type](auto& component) {
				return component->IsClassType(classType);
			});

		bool success = index != components.end();

		if (success)
			components.erase(index);

		return success;
	}

	//***************
	// GameObject::GetComponents
	// returns a vector of pointers to the the requested component template type following the same match criteria as GetComponent
	// NOTE: the compiler has the option to copy-elide or move-construct componentsOfType into the return value here
	// TODO: pass in the number of elements desired (eg: up to 7, or only the first 2) which would allow a std::array return value,
	// except there'd need to be a separate fn for getting them *all* if the user doesn't know how many such Components the GameObject has
	// TODO: define a GetComponentAt<ComponentType, int>() that can directly grab up to the the n-th component of the requested type
	//***************
	template< class ComponentType >
	std::vector< ComponentType* > GameObject::GetComponents() {
		std::vector< ComponentType* > componentsOfType;

		for (auto&& component : components) {
			if (component->IsClassType(ComponentType::Type))
				componentsOfType.emplace_back(static_cast<ComponentType*>(component.get()));
		}

		return componentsOfType;
	}

	//***************
	// GameObject::RemoveComponents
	// returns the number of successful removals, or 0 if none are removed
	//***************
	template< class ComponentType >
	int GameObject::RemoveComponents() {
		if (components.empty())
			return 0;

		int numRemoved = 0;
		bool success = false;

		do {
			auto& index = std::find_if(components.begin(),
				components.end(),
				[classType = ComponentType::Type](auto& component) {
					return component->IsClassType(classType);
				});

			success = index != components.end();

			if (success) {
				components.erase(index);
				++numRemoved;
			}
		} while (success);

		return numRemoved;
	}
}

using namespace rtti;

const std::size_t Component::Type = std::hash<std::string>()(TO_STRING(Component));

CLASS_DEFINITION(Component, Collider)
CLASS_DEFINITION(Collider, BoxCollider)
CLASS_DEFINITION(Component, RenderImage)

#define MORE_CODE 0

//	TEMP
class Performance
{
public:
	Performance()
	{
		QueryPerformanceFrequency(&timer);
		QueryPerformanceCounter(&start);
	}
	~Performance()
	{
		QueryPerformanceCounter(&end);

		DeltaTime = (end.QuadPart - start.QuadPart) / (float)timer.QuadPart;
		auto t = std::to_wstring(DeltaTime) + L"\n";
		::OutputDebugString(t.c_str());
	}

private:
	LARGE_INTEGER timer{}, start{}, end{};
	float DeltaTime = 0.f;
};

class Test
{

};

template< class T >
T aa()
{
	return std::unique_ptr<T>(nullptr);
}

int main(int argc, const char* argv) {

	using namespace rtti;

	auto t = aa<Test>();

	if (t)
	{
		int a = 3;
	}
	else
	{
		int b = 4;
	}
	{
		Performance p;
		GameObject test;

		// AddComponent test
		test.AddComponent< Component >("t");
		test.AddComponent< Collider >("1");
		test.AddComponent< Collider >("2");
		test.AddComponent< Collider >("3");
		test.AddComponent< BoxCollider >("4");

		for (int i = 0; i < 10000; i++)
		{
			auto& boxColliderRef1 = test.GetComponent< BoxCollider >();
			int a = 3;
		}
	}

	return 0;
}


enum class ActorState
{
	Active,
	Paused,
	Dead,
};

class Component : public std::enable_shared_from_this<Component>
{
public:
	Component()
	{

	}

	virtual ~Component()
	{

	}
};

class TileComponent : public Component
{
public:
	TileComponent()
	{

	}
	virtual ~TileComponent()
	{

	}
};

class SpriteComponent : public Component
{
public:
	SpriteComponent()
	{

	}
	virtual ~SpriteComponent()
	{

	}
};

class Actor : public std::enable_shared_from_this<Actor>
{
public:
	Actor()
	{

	}
	virtual ~Actor()
	{

	}

public:

public:
	template<typename T>
	std::shared_ptr<T>			AddComponent()
	{
		assert(FindComponent<T>() == false);

		std::shared_ptr<Component> component = std::make_shared<T>();

		std::string name = typeid(T).name();
		_components[name] = component;

		return std::static_pointer_cast<T>(component);

	}

	template<typename T>
	void			RemoveComponent()
	{
		if (FindComponent<T>)
		{
			std::string name = typeid(T).name();
			_components.erase(name);
		}
	}

	template<typename... T>
	decltype(auto)		GetComponent()
	{
		std::vector<const char*> names = { typeid(T).name()... };

		if (names.size() > 0)
		{
			for (auto& name : names)
			{
				auto iter = _components.find(name);
				if (iter != _components.end())
					return iter->second;
			}
		}

		return std::shared_ptr<Component>();
	}

private:
	template<typename T>
	bool	FindComponent()
	{
		std::string name = typeid(T).name();
		auto iter = _components.find(name);
		if (iter != _components.end())
			return true;

		return false;
	}

public:

	ActorState			GetState() const { return _state; }
	void				SetState(ActorState state) { _state = state; }


private:
	ActorState			_state = ActorState::Active;


	//	TEMP
	std::unordered_map<std::string, std::shared_ptr<Component>>	_components;
};

class Test
{


};

#define TO_STRING( x ) #x

int main(void)
{
	std::vector<std::size_t> v;
	auto hash1 = std::hash< std::string >()(TO_STRING(Test)); 
	auto hash2= std::hash< std::string >()(TO_STRING(Test)); 
	v.push_back(hash1);

	return 0;
}

	std::shared_ptr<Actor> actor = std::make_shared<Actor>();

	auto tile = actor->AddComponent<TileComponent>();
	auto tile2 = actor->AddComponent<TileComponent>();
	//auto sprite = actor->AddComponent<SpriteComponent>();

	//auto sprtie = actor->GetComponent<TileComponent>();
	auto sprtie2 = actor->GetComponent<TileComponent, SpriteComponent>();
	int a = 3;
	
	//HWND hwnd;
	//DWORD pid;
	//hwnd = FindWindow(0, L"echo_library");
	//if (hwnd == 0)
	//{
	//	system("pause");
	//	return 0;
	//}
	//GetWindowThreadProcessId(hwnd,&pid);
	//HANDLE hProcess = OpenProcess(PROCESS_VM_OPERATION | PROCESS_VM_WRITE | PROCESS_VM_READ, FALSE, pid);
	//CpuUsage cpuMonitor;

	//while (true)
	//{
	//	Sleep(1000);
	//	

	//	cpuMonitor.Update();
	//	
	//	printf("processor Total  %f%%\n", cpuMonitor.ProcessorTotal());
	//	printf("processor User   %f%%\n", cpuMonitor.ProcessorUser());
	//	printf("processor Kernel %f%%\n", cpuMonitor.ProcessKernel());
	//	printf("process   Total  %f%%\n", cpuMonitor.ProcessTotal());
	//	printf("process   User   %f%%\n", cpuMonitor.ProcessUser());
	//	printf("process   Kernel %f%%\n", cpuMonitor.ProcessKernel());
	//	printf("process Total private Bytes %llu\n", cpuMonitor.PrivateByteTotal());
	//	printf("process       private Bytes %llu\n", cpuMonitor.PrivateByte());
	//	printf("process Total nonPaged Bytes %llu\n", cpuMonitor.ProcessNonPagedByteTotal());
	//	printf("process       nonPaged Bytes %llu\n", cpuMonitor.ProcessNonPagedByte());
	//	printf("memory available Mbytes %llu\n", cpuMonitor.AvailableMBytes());
	//	printf("memory nonpaged   bytes %llu\n", cpuMonitor.PoolNonpagedBytes());
	//}

//#include <Windows.h>
//#include <DbgHelp.h>
//
//#include <iostream>
//#include <string>
//using namespace std;
//#pragma comment(lib, "dbghelp.lib")
//
//thread_local uint32_t LThreadId = 0;
//
////	TEMP
//class ThreadCallStack
//{
//public:
//	ThreadCallStack()
//	{
//		symbol = reinterpret_cast<SYMBOL_INFO*>(::calloc(symbolSize, 1));
//	}
//	~ThreadCallStack()
//	{
//		if (symbol)
//		{
//			::free(symbol);
//			symbol = nullptr;
//		}
//	}
//
//public:
//	void Print()
//	{
//		if (symbol == nullptr)
//			return;
//
//		::memset(symbol, 1, symbolSize);
//
//		process = ::GetCurrentProcess();
//		::SymInitialize(process, NULL, TRUE);
//
//		frames = CaptureStackBackTrace(0, SMaxStackDepth, stack, NULL);
//
//		if (symbol == nullptr)
//			return;
//
//		symbol->MaxNameLen = MAX_PATH;
//		symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
//
//		std::cout << "==========================================================================================" << std::endl;
//		std::cout << "ThreadId : " << LThreadId << "Call Stack" << std::endl;
//		for (uint16_t i = 0; i < frames; i++)
//		{
//			::SymFromAddr(process, reinterpret_cast<DWORD64>(stack[i]), 0, symbol);
//
//			BOOL ret = ::SymGetLineFromAddr64(process, reinterpret_cast<DWORD64>(stack[i]), &displacement, &line);
//
//			if (ret)
//			{
//				std::cout << line.FileName << line.LineNumber << " - ";
//				std::cout << symbol->Name << std::endl;
//				//std::cout << std::hex << symbol->Address << std::endl;
//			}
//			else
//			{
//				std::cout << symbol->Name << std::endl;
//				//std::cout << std::hex << symbol->Address << std::endl;
//			}
//		}
//		std::cout << "==========================================================================================" << std::endl;
//	}
//
//private:
//	static const int16_t		SMaxStackDepth = 16;
//	void*						stack[SMaxStackDepth] = { 0, };
//	uint16_t					frames = 0;
//	SYMBOL_INFO*				symbol = nullptr;;
//	uint16_t					symbolSize = sizeof(SYMBOL_INFO) + MAX_PATH * sizeof(char);
//	HANDLE						process;
//	DWORD						displacement = 0;
//	IMAGEHLP_LINE64				line;
//
//};
//
//int main()
//{
//	ThreadCallStack call;
//	call.Print();
//	return 0;
//}
//
//#include <thread>
//
//class Local
//{
//public:
//	void Test()
//	{
//
//	}
//};
//
//void Ll()
//{
//
//}
//
//class Thread
//{
//public:
//	Thread()
//	{
//		_t = std::move(std::thread([]() {
//				Local l;
//		l.Test();
//		printStack();
//
//			}));
//	}
//	~Thread()
//	{
//
//	}
//
//public:
//	std::thread _t;
//};
//
//int main()
//{
//	Thread t;
//
//	t._t.join();
//
//	//printStack();
//	return 0;
//}
//


//#define _CRTDBG_MAP_ALLOC
//
//
//
//#include <tchar.h>
//
//#include <memory>
//
//
//
//#include <crtdbg.h>
//
//
//
//int _tmain(int argc, _TCHAR* argv[])
//
//{
//
//    _CrtSetDbgFlag(
//
//        _CRTDBG_CHECK_ALWAYS_DF
//
//        | _CRTDBG_LEAK_CHECK_DF
//
//        | _CRTDBG_ALLOC_MEM_DF
//
//        | _CRTDBG_DELAY_FREE_MEM_DF
//
//    );
//
//
//
//
//
//    int* p = new int[10];
//
//    const int size = sizeof(int) * 10;
//
//
//
//    //p[9]까지유효하기때문에p[10]에무언가를기록하면
//
//    //Memory Corruption이다. 커럽션이발생한이후에
//
//    //_CrtCheckMemory()를실행하면false를리턴한다.
//
//    _ASSERT(true == _CrtCheckMemory());
//
//    p[10] = 0;
//
//    _ASSERT(false == _CrtCheckMemory());
//
//    delete[] p;
//
//    return 0;
//
//}
//#include <iostream>
//#include <Windows.h>
//#include <memory>
//
//#define _CRTDBG_MAP_ALLOC
//#include <cstdlib>
//#include <crtdbg.h>
//#include <functional>
//class BBBB;
//class Test
//{
//public:
//	Test()
//	{
//
//	}
//	~Test()
//	{
//
//	}
//public:
//	void Clear()
//	{
//		std::cout << "Clear" << std::endl;
//		f = nullptr;
//	}
//public:
//	std::function<void()> f;
//	std::shared_ptr<BBBB> b;
//};
//
//class BBBB
//{
//public:
//	std::shared_ptr<Test> t;
//};
//
//int main()
//{
//	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
//
//	auto test = std::make_shared<Test>();
//	test->f = [pp = test]() {
//		pp->Clear();
//	};
//
//	test->f();
//
//	return 0;
//}
////
////#include <windows.h>
////#include "StackWalker.h"
//
////void Func5() { StackWalker sw; sw.ShowCallstack(); }
////void Func4() { Func5(); }
////void Func3() { Func4(); }
////void Func2() { Func3(); }
////void Func1() { Func2(); }
////
////#include <unordered_map>
////#include <string>
////class Test
////{
////public:
////    void Clear()
////    {
////        _texts.clear();
////    }
////public:
////    std::unordered_map<std::string, std::string>				_texts;
////};
////Test t;
////int main()
////{
////    t._texts.insert(std::make_pair("11", "22"));
////    t.Clear();
////
////    return 0;
////}
////#pragma comment(lib, "Debug\\ServerCore.lib")
////#include <CorePch.h>
////
////class Knight
////{
////public:
////	Knight()
////	{
////
////	}
////	Knight(int h, int a)
////		: hp(h), attack(a)
////	{
////
////	}
////	~Knight()
////	{
////		std::cout << "헤헤" << std::endl;
////	}
////public:
////	int hp = 0;
////	int attack = 0;
////};
////
////template<typename T>
////class RingBuffer
////{
////public:
////	RingBuffer() = default;
////	RingBuffer(size_t capacity)
////	{
////		_buffer.resize(capacity);
////	}
////
////	bool Push(T& value) 
////	{
////		auto const writePos = _writePos.load(std::memory_order_relaxed);
////		auto nextWritePos = writePos + 1;
////
////		if (nextWritePos == _buffer.size()) 
////			nextWritePos = 0;
////		
////		//	해당 명령 뒤에 오는 모든 메모리 명령들이 해당 명령 위로 재배치 되는 것을 금지 
////		if (nextWritePos == _readPos.load(std::memory_order_acquire))
////			return false;
////
////		_buffer[writePos] = value;
////		//	 해당 명령 이전의 모든 메모리 명령들이 해당 명령 이후로 재배치 되는 것을 금지 
////		_writePos.store(nextWritePos, std::memory_order_release);
////		return true;
////	}
////
////	bool Pop(OUT T& value) 
////	{
////		auto const readPos = _readPos.load(std::memory_order_relaxed);
////
////		if (readPos == _writePos.load(std::memory_order_acquire))
////			return false;
////
////		value = _buffer[readPos];
////		auto nextReadPos = readPos + 1;
////
////		if (nextReadPos == _buffer.size())
////			nextReadPos = 0;
////		
////		_readPos.store(nextReadPos, std::memory_order_release);
////		return true;
////	}
////
////public:
////	std::vector<T>		_buffer{};
////	alignas(64)			std::atomic<size_t> _readPos{ 0 };
////	alignas(64)			std::atomic<size_t> _writePos{ 0 };
////
////};
////
////
////
////void bench()
////{
////	const size_t queueSize = 100000;
////	const int64_t iters = 100000000;
////
////	RingBuffer<int> q(queueSize);
////	auto t = std::thread([&] {
////		for (int i = 0; i < iters; ++i) {
////			int val;
////			while (!q.Pop(val))
////				;
////			if (val != i) {
////				throw std::runtime_error("");
////			}
////		}
////		});
////
////
////	auto start = std::chrono::steady_clock::now();
////	for (int i = 0; i < iters; ++i) {
////		while (!q.Push(i))
////			;
////	}
////	while (q._readPos.load(std::memory_order_relaxed) !=
////		q._writePos.load(std::memory_order_relaxed))
////		;
////
////	auto stop = std::chrono::steady_clock::now();
////	t.join();
////
////	std::cout << iters * 1000000000 /
////		std::chrono::duration_cast<std::chrono::nanoseconds>(stop -
////			start)
////		.count()
////		<< " ops/s" << std::endl;
////}
////
////
////class Test
////{
////public:
////	int a = 3;
////};
////int main()
////{
////	auto t = ObjectPool<Test>::Pop();
////	ObjectPool<Test>::Push(t);
////	//bench();
////
////	//Registry t;
////	
////	//t.OpenKey(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\TEST_REG"));
////	//t.DeleteKey(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\TEST_REG"));
////
////	//t.SetValue(_T("test"), 3);
////
////	//DWORD value = 0;
////	//t.GetValue(_T("test"), &value);
////	//t.DeleteValue(_T("test"));
////	//t.DeleteKey(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\TEST_REG"));
////	//IniFileParser i;
////	//DWORD port = 1001;
////	//i.SetFileName(_T("C:\\CSI\\Init.ini"));
////	//std::wstring t;
////	////t = L"TEST";
////	////i.SetSectionValue(_T("SERVER CONFIG"), _T("PORT"), t);
////	//i.GetSectionValue(_T("SERVER CONFIG"), _T("PORT"), t);
////	//int a = 3;
////	//DWORD p2 = 0;
////	//i.GetSectionValue(_T("SERVER CONFIG"), _T("PORT"), &p2);
////
////	/*Logger l;
////	l.Log(_T("test : %s"), _T("local"));*/
////
////	//auto k = cnew<Knight>(10, 20);
////	//cdelete<Knight>(k);
////
////	/*auto k = ObjectPool<Knight>::Pop(10, 20);
////	ObjectPool<Knight>::Push(k);*/
////
////	//auto k = ObjectPool<Knight>::MakeShared(10, 20);
////	
////	//std::thread t1([]() {
////	//		while (true)
////	//		{
////	//			auto k = ObjectPool<Knight>::Pop(10, 20);
////	//			std::cout << k->hp << std::endl;
////	//			ObjectPool<Knight>::Push(k);
////	//		}
////	//	});
////
////	//std::thread t2([]() {
////	//		while (true)
////	//		{
////	//			auto k = ObjectPool<Knight>::Pop(20, 20);
////	//			std::cout << k->hp << std::endl;
////	//			ObjectPool<Knight>::Push(k);
////	//		}
////	//	});
////
////	//t1.join();
////	//t2.join();
////	
////	//RingBuffer<Knight> r{ 30 };
////	//Knight k{ 3,3 };
////	//r.Push(k);
////
////	//Knight t;
////	//r.Pop(t);
////
////
////
////	return 0;
////}
////

