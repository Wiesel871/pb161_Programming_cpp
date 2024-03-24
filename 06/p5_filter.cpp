#include <cassert>
#include <cstddef>
#include <cstdio>
#include <set>
#include <memory>

/* This exercise will be another take on a set of numbers. This
 * time, we will add a capability to filter the numbers on output.
 * It will be possible to change the filter applied to a given set
 * at runtime. */

/* The «base class» for representing filters will contain a single
 * pure ‹virtual› method, ‹accept›. The method should be marked
 * ‹const›. */

class filter {
    public:
    virtual bool accept(int) const {
        return true;
    }
};

/* The ‹set› (which we will implement below) will «own» the filter
 * instance and hence will use a ‹unique_ptr› to hold it. */

using filter_ptr = std::unique_ptr< filter >;

/* The ‹set› should have standard methods: ‹add› and ‹has›, the
 * latter of which will respect the configured filter (i.e. items
 * rejected by the filter will always test negative on ‹has›). The
 * method ‹set_filter› should set the filter. If no filter is set,
 * all numbers should be accepted. Calling ‹set_filter› with a
 * ‹nullptr› argument should clear the filter. */

/* Additionally, ‹set› should have ‹begin› and ‹end› methods (both
 * ‹const›) which return very simple iterators that only provide
 * «dereference» to an ‹int› (value), pre-increment and inequality.
 * It is a good idea to keep «two» instances of
 * ‹std::set< int >::iterator› in attributes (in addition to a
 * pointer to the output filter): you will need to know, in the
 * pre-increment operator, that you ran out of items when skipping
 * numbers which the filter rejected. */
class set;

class set_iterator {
    public:
    const set *s;
    std::set<int>::iterator it;
    std::size_t counter = 0;

    set_iterator(const set *s, std::set<int>::iterator it, std::size_t c) : s{s}, it{it}, counter{c} {}

    bool operator!=(const set_iterator &r) const {
        return *it != *r.it || it != r.it || counter != r.counter || s != r.s;
    };

    int operator*() {
        return *it;
    }

    set_iterator &operator++();
};

class set {
    public:
    filter_ptr _filter = nullptr;
    std::set<int> s = {};


    friend class set_iterator;

    bool has(int x) const {
        return ((!_filter) || _filter->accept(x)) && s.contains(x);
    }

    void add(int x) {
        if (!_filter || _filter->accept(x))
        s.insert(x);
    }

    void set_filter(filter_ptr p) {
        _filter.release();
        _filter.swap(p);
        p.release();
    }

    set_iterator begin() const {
        if (!_filter)
            return {this, s.begin(), 0};
        auto it = s.begin();
        std::size_t c = 0;
        while (!_filter->accept(*it)) {
            ++it;
            ++c;
        }
        return {this, it, c};
    }

    set_iterator end() const {
        return {this, s.end(), s.size()};
    }
};

set_iterator &set_iterator::operator++() {
    ++it;
    ++counter;
    if (!s->_filter) {
        return *this;
    }
    
    while (it != s->s.end() && !(s->_filter->accept(*it))) {
        ++it;
        ++counter;
    }
    return *this;
}


/* Finally, implement a filter that only accepts odd numbers. */

class odd : public filter {
    bool accept(int x) const override {
        return x % 2;
    }
};

int main()
{
    set s;
    for ( int i : { 1, 2, 3, 4 } )
        s.add( i );

    assert( s.has( 1 ) );
    assert( s.has( 2 ) );

    int j = 0;
    for ( int i : s ) {
        assert( i == ++j );
    }
    assert( j == 4 );

    s.set_filter( std::make_unique< odd >() );
    assert(  s.has( 1 ) );
    assert( !s.has( 2 ) );

    for ( int i : s ) {
        assert( i % 2 == 1 );
    }

    j = 0;
    for ( int i : s )
        j ++, i ++;
    assert( j == 2 );

    s.add( 0 );
    assert( *s.begin() == 1 );

    return 0;
}
