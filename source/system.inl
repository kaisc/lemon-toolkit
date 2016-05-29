// @date 2016/05/28
// @author Mao Jingkai(oammix@gmail.com)

// INCLUDED METHODS OF SYSTEM MANAGER

template<typename T>
System::Type SystemTrait<T>::type()
{
    static Type cls = s_type_counter ++;
    return cls;
}

template<typename C, typename ... Args>
void RequireComponents<C, Args...>::attach(SystemManager& sys)
{
    auto& evt = sys.get_event_manager();
    evt.subscribe<EvtComponentAdded<C>>(*this);
}

template<typename C, typename ... Args>
void RequireComponents<C, Args...>::detach(SystemManager& sys)
{
    auto& evt = sys.get_event_manager();
    evt.unsubscribe<EvtComponentAdded<C>>(*this);
}

template<typename C, typename ... Args>
void RequireComponents<C, Args...>::receive(const EvtComponentAdded<C>& evt)
{
    add_component<Args...>(evt.entity);
}

template<typename C, typename ... Args>
template<typename D>
void RequireComponents<C, Args...>::add_component(Entity ent)
{
    if( !ent.has_component<D>() )
        ent.add_component<D>();
}

template<typename C, typename ... Args>
template<typename D, typename D1, typename ... Tails>
void RequireComponents<C, Args...>::add_component(Entity ent)
{
    add_component<D>(ent);
    add_component<D1, Tails...>(ent);
}

EventManager& SystemManager::get_event_manager()
{
    return m_event_manager;
}

EntityManager& SystemManager::get_entity_manger()
{
    return m_entity_manager;
}

template<typename S, typename ... Args>
S* SystemManager::ensure(Args && ... args)
{
    auto found = m_systems.find(S::type());
    if( found == m_systems.end() )
    {
        auto sys = new (std::nothrow) S(std::forward<Args>(args) ...);
        m_systems.insert(std::make_pair(S::type(), sys));
        return sys;
    }

    return found->second;
}


template<typename S, typename ... Args>
S* SystemManager::add(Args && ... args)
{
    auto found = m_systems.find(S::type());
    assert( found == m_systems.end() && "[ECS] duplicated system.");

    auto sys = new (std::nothrow) S(std::forward<Args>(args) ...);
    m_systems.insert(std::make_pair(S::type(), sys));
    return sys;
}

template<typename S>
void SystemManager::remove()
{
    auto found = m_systems.find(S::type());
    if( found != m_systems.end() )
    {
        delete found->second;
        m_systems.erase(found);
    }
}

template<typename S>
S* SystemManager::get()
{
    auto found = m_systems.find(S::type());
    if( found != m_systems.end() )
        return static_cast<S*>(found->second);
    return nullptr;
}