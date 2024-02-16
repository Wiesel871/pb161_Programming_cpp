class job
{
    int _id;
    int _owner;
    int _pages;

public:

    job( int id, int owner, int pages )
        : _id( id ), _owner( owner ), _pages( pages )
    {}

    job( job && ) = default;
    job( const job & ) = delete;
    job &operator=( const job & ) = delete;

    int id() const { return _id; }
    int page_count() const { return _pages; }
    int owner() const { return _owner; }
};

class queue
{
    std::map< int, job > _jobs;
    int _count = 0;
public:

    int dequeue()
    {
        const auto &item = *_jobs.begin();
        int id = item.first;
        _count -= item.second.page_count();
        _jobs.erase( id );
        return id;
    }

    void enqueue( job &&j )
    {
        int id = j.id();
        _count += j.page_count();
        _jobs.emplace( id, std::move( j ) );
    }

    job release( int id )
    {
        job rv( std::move( _jobs.at( id ) ) );
        _jobs.erase( id );
        _count -= rv.page_count();
        return rv;
    }

    int page_count() const
    {
        return _count;
    }
};
