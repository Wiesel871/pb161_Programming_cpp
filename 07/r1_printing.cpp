#include <cassert>
#include <vector>
#include <map>

/* Jobs need resources (printing credits, where 1 page = 1 credit)
 * which must be reserved when the job is queued, but are only
 * consumed at actual printing time; jobs can be moved between
 * queues (printers) by the system, and jobs that are still in the
 * queue can be aborted. */

/* The class ‹job› represents a document to be printed, along with
 * resources that have already been earmarked for its printing.
 *
 *  • The constructor should take a numeric identifier, the id of
 *    the user who owns the job, and the number of pages (= credits
 *    allocated for the job),
 *  • method ‹owner› should return the id of the owner,
 *  • method ‹page_count› should return the number of pages. */

class job;

/* A single ‹queue› instance represents a printer. It should have
 * the following methods:
 *
 *  • ‹dequeue›: consume (print) the «oldest» job in the queue and
 *    return its ‹id›,
 *  • ‹enqueue›: add a job to the queue,
 *  • ‹release( id )›: remove the job given by ‹id› from the queue
 *    and return it to the caller,
 *  • ‹page_count›: number of pages in the queue.
 *
 * You can assume that oldest job has the lowest ‹id›. */

class queue;

int main()
{
    static_assert( !std::is_copy_assignable_v< job > );
    static_assert( !std::is_copy_constructible_v< job > );

    class insufficient_credit {};

    class print_sys
    {
        int _last_id = 0;

        std::vector< queue > _printers;
        std::map< int, int > _credit;
        std::map< int, int > _jobs;

    public:
        int print( int user, int pages )
        {
            if ( _credit[ user ] < pages )
                throw insufficient_credit();

            assert( !_printers.empty() );
            int qid = 0;

            for ( std::size_t i = 0; i < _printers.size(); ++i )
                if ( _printers[ i ].page_count() < _printers[ qid ].page_count() )
                    qid = i;

            int jid = _last_id ++;
            _printers[ qid ].enqueue( job( jid, user, pages ) );
            _jobs[ jid ] = qid;
            _credit[ user ] -= pages;
            return jid;
        }

        bool abort( int jid )
        {
            if ( !_jobs.count( jid ) )
                return false;

            auto &print_queue = _printers[ _jobs[ jid ] ];
            job j = print_queue.release( jid );
            _credit[ j.owner() ] += j.page_count();
            _jobs.erase( jid );
            return true;
        }

        int add_printer()
        {
            _printers.emplace_back();
            return _printers.size() - 1;
        }

        void add_credit( int user, int n )
        {
            _credit[ user ] += n;
        }

        void printing_done( int qid )
        {
            if ( _printers[ qid ].page_count() > 0 )
            {
                int id = _printers[ qid ].dequeue();
                assert( _jobs.count( id ) );
                assert( _jobs[ id ] == qid );
                _jobs.erase( id );
            }
        }
    };

    auto assert_throws = []( auto func )
    {
        try
        {
            func();
            assert( 0 );
        }
        catch ( insufficient_credit ) {};
    };

    print_sys psys;

    int p_1 = psys.add_printer(),
        p_2 = psys.add_printer();

    psys.add_credit( 0xa11ce, 7 );
    psys.add_credit( 0xb0b, 3 );

    int j_1 = psys.print( 0xa11ce, 4 ); // j_1 = 4 | 0
    assert_throws( [&]{ psys.print( 0xa11ce, 5 ); } );
    int j_2 = psys.print( 0xa11ce, 2 ); // j_1 = 4 | j_2 = 2
    assert_throws( [&]{ psys.print( 0xb0b, 7 ); } );
    assert(  psys.abort( j_1 ) );       // 0 | j_2 = 2
    assert( !psys.abort( j_1 ) );
    int j_3 = psys.print( 0xb0b, 3 );   // j_3 = 3 | j_2 = 2
    int j_4 = psys.print( 0xa11ce, 5 ); // j_3 = 3 | j_2 + j_4 = 7

    psys.printing_done( p_2 );          // j_3 = 3 | j_4 = 5
    assert( !psys.abort( j_2 ) );
    assert(  psys.abort( j_4 ) );       // j_3 = 3 | 0
    int j_5 = psys.print( 0xa11ce, 2 ); // j_3 = 3 | j_5 = 2

    psys.printing_done( p_1 );          // 0 | j_5 = 2
    assert( !psys.abort( j_3 ) );
    psys.printing_done( p_2 );          // 0 | 0
    assert( !psys.abort( j_5 ) );

    return 0;
}
