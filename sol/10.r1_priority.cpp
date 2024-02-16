using _pub_::task;

bool sched_cmp( const task &a, const task &b )
{
    auto key = []( const task &t )
    {
        return std::pair( t.priority, -t.id );
    };

    return key( a ) < key( b );
}

struct sched_queue
{
    std::vector< task > tasks;
    bool complete = true;

    void fix()
    {
        if ( !complete )
            std::push_heap( tasks.begin(), tasks.end(), sched_cmp );
        complete = true;
    }

    void add( task t )
    {
        fix();
        tasks.push_back( t );
        complete = false;
        fix();
    }

    task &demote()
    {
        fix();
        std::pop_heap( tasks.begin(), tasks.end(), sched_cmp );
        complete = false;
        -- tasks.back().priority;
        return tasks.back();
    }

    void reset()
    {
        for ( auto &t : tasks )
            t.priority = t.static_priority;
        std::make_heap( tasks.begin(), tasks.end(), sched_cmp );
        complete = true;
    }

    const task &peek() const
    {
        const auto &top = tasks.front(), &demoted = tasks.back();

        if ( complete || sched_cmp( demoted, top ) )
            return top;
        else
            return demoted;
    }
};
