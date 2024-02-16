using _pub_::mutex;

class lock
{
    mutex *_mutex;
public:
    lock( mutex &m );
    ~lock();

    lock( const lock & ) = delete;
    lock &operator=( const lock & ) = delete;

    lock( lock && );
    lock &operator=( lock && );
};

lock::lock( mutex &m )
    : _mutex( &m )
{
    _mutex->lock();
}

lock::~lock()
{
    if ( _mutex )
        _mutex->unlock();
}

lock::lock( lock &&o )
    : _mutex( o._mutex )
{
    o._mutex = nullptr;
}

lock &lock::operator=( lock &&o )
{
    if ( _mutex )
        _mutex->unlock();
    _mutex = o._mutex;
    o._mutex = nullptr;
    return *this;
}
