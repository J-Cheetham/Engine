#pragma once

#include "Engine\Core.h"

#include <string>
#include <functional>

namespace Engine {

	//Events are currently blocking. When an event occurs it immediately
	// gets dispatched and must be dealt with right away. For the future
	// buffering the events in an event buss and process them during the 
	// "event" part of the update stage is a better strategy.

	enum class EventType
	{
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		AppTick, AppUpdate, AppRender,
		KeyPressed, KeyReleased,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};

	// May want to filter events as they get sent to the OnEvent class
	// Use of BIT field so an event can be part of multiple categories
	enum EventCategory
	{
		None = 0,
		EventCategoryApplication = BIT(0),
		EventCategoryInput = BIT(1),
		EventCategoryKeyboard = BIT(2),
		EventCategoryMouse = BIT(3),
		EventCategoryMouseButton = BIT(4)
	};

#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() {return EventType::##type;}\
							   virtual EventType GetEventType() const override {return GetStaticType();}\
							   virtual const char* GetName() const override {return #type;}

#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override {return category;}

	class ENG_API Event
	{
		//friend class EventDispatcher;

	public:
		virtual EventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;
		virtual int GetCategoryFlags() const = 0;
		virtual std::string ToString() const { return GetName(); }

		inline bool IsInCategory(EventCategory category)
		{
			//Returns 0 if not in the category
			//Returns anything other than zero, it's part of the category
			return GetCategoryFlags() & category;
		}
	protected:
		bool m_Handled = false;
	};

	//Dispatches events based on the event type really easily
	class EventDispatcher
	{
		//EventFn is a std::function that return bool and takes a T&
		template<typename T>
		using EventFn = std::function<bool(T&)>;
	public:
		//Can take any type of event
		EventDispatcher(Event& event)
			:m_Event(event)
		{

		}

		template<typename T>
		bool Dispatch(EventFn<T> func)
		{
			//Can call Dispatch with different functions. If the type of the function 
			//matches the type of the event then it will call the function with that event
			//Does nothing otherwise
			if (m_Event.GetEventType() == T::GetStaticType())
			{
				m_Event.m_Handled = func(*(T*)&m_Event);
				return true;
			}
			return false;
		}

	private:
		Event& m_Event;
	};

	//Allows us to print out events for logging
	inline std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		return os << e.ToString();
	}
}