#include <string_view>
#include <vector>
#include <cassert>

/* † In this demo, we will implement a full-featured «input
 * iterator»: the most basic kind of iterator that can be used to
 * obtain values (as opposed to updating them, as done by an «output
 * iterator»).
 *
 * A common task is to split a string into words, lines or other
 * delimiter-separated items. This is one of the cases where the
 * standard library does not offer any good solutions: hence, we
 * will roll our own. The class will be called ‹splitter› and will
 * take 2 parameters: the string (‹string_view› to be exact) to be
 * split, and the delimiter (for simplicity limited to a single
 * character).
 *
 * The splitter is based on ‹string_view› to make the whole affair
 * ‘zero-copy’: the string data is never copied. The downside is
 * that the input string (the one being split) must ‘outlive’ the
 * ‹splitter› instance. */

/* We will re-use the ‹split› function from previous demo and use it
 * as the ‘workhorse’ of the ‹splitter›. */

using split_view = std::pair< std::string_view, std::string_view >;

split_view split( std::string_view s, char delim )
{
    size_t idx = s.find( delim );
    if ( idx == s.npos )
        return { s, {} };
    else
        return { s.substr( 0, idx ), s.substr( idx + 1, s.npos ) };
}

/* The ‹splitter› class itself doesn't do much: its main role is to
 * create iterators, via ‹begin› and ‹end›. To this end, it must of
 * course remember the input string and the delimiter. */

struct splitter
{
    using value_type = std::string_view;
    std::string_view _str;
    char _delim;

    /* Real iterators «must» provide ‹operator->› – the one that is
     * invoked when we say ‹iter->foo()›. For this particular
     * use-case, this is a little vexing: ‹operator->› «must» return
     * either a raw pointer, or an instance of a class with
     * overloaded ‹operator->›. Clearly, that chain must end
     * somewhere – sooner or later, we must have an «address» of the
     * item to which the iterator points.
     *
     * This is inconvenient, because we want to construct that item
     * ‘on the fly’ – whenever it is needed – and return it from the
     * dereference operator (unary ‹*›) «by value», not by
     * reference.
     *
     * The above two requirements are clearly contradictory: as you
     * surely know, returning the address of a local variable won't
     * do. This is where ‹proxy› comes into play: its role is to
     * hold a copy of the item that has sufficiently long lifetime.
     *
     * Later, we will return ‹proxy› instances by value, hence
     * ‹proxy› itself will be a temporary object. Since temporary
     * objects live until the ‘end of statement’ (i.e. until the
     * nearest semicolon, give or take), we can return the address
     * of its own attribute. That address will be good until the
     * entire statement finishes executing, which is good enough:
     * that means when we write ‹iter->foo()›, the proxy constructed
     * by ‹operator->›, and hence the ‹string_view› stored in its
     * attribute, will still exist when ‹foo› gets executed. */

    struct proxy
    {
        value_type v;
        value_type *operator->() { return &v; }
    };

    struct iterator
    {
        /* There are 5 ‘nested types’ that iterators must provide.
         * The probably most important is ‹value_type›, which is the
         * type of the element that we get when we dereference the
         * iterator. */

        using value_type = splitter::value_type;

        /* The ‹iterator_category› type describes what kind of
         * iterator this is, so that generic algorithms can take
         * advantage of the extra guarantees that some iterators
         * provide. This is a humble «input iterator», and hence
         * gets ‹std::input_iterator_tag› as its category. */

        using iterator_category = std::input_iterator_tag;

        /* The remaining 3 types exist to make writing generic
         * algorithms somewhat easier. The ‹difference_type› is what
         * you would get by subtracting two iterators – the
         * ‘default’ is ‹ptrdiff_t›, so we use that, even though our
         * iterators cannot be subtracted. */

        using difference_type = std::ptrdiff_t;

        /* The last two are ‘decorated’ versions of ‹value_type›: a
         * pointer, which is straightforward (but do remember to take
         * ‹const›-ness into account)… */

        using pointer = value_type *;

        /* … and a reference (same ‹const› caveat applies). However,
         * you might find it surprising that the latter is not
         * actually a reference type in this case. Why? Because
         * ‹reference› is defined as ‘what the dereference operator
         * returns’, and our ‹operator*› returns a value, not a
         * reference. Input iterators have an exception here: all
         * higher iterator types (forward, bidirectional and random)
         * must make ‹reference› an actual reference type. */

        using reference = value_type;

        /* Now, finally, for the implementation. The data members
         * (and the constructor, and the assignment operator) are
         * all straightforward. The ‹_str› attribute represents the
         * reminder of the string that still needs to be split, and
         * will be an empty string for the ‹end› iterator. Remember
         * that ‹string_view› does not hold any data, so we are
         * «not» making copies of the input string. */

        std::string_view _str;
        char _delim;

        iterator( std::string_view s, char d )
            : _str( s ), _delim( d )
        {}

        iterator( const iterator & ) = default;
        iterator &operator=( const iterator & ) = default;

        /* The pre-increment and post-increment operators are
         * reasonably simple. As is usual, we implement the latter
         * in terms of the former. */

        iterator &operator++()
        {
            _str = split( _str, _delim ).second;
            return *this;
        }

        iterator operator++( int )
        {
            auto orig = *this;
            ++*this;
            return orig;
        }

        /* Dereference would be unremarkable, except for the part
         * where we return a value instead of a reference (we could
         * use the ‹reference› nested type here to make it clear we
         * are adhering to iterator requirements, but that would be
         * likely more confusing, considering how ‹reference› is not
         * a reference). Do note the ‹const› here. */

        value_type operator*() const
        {
            return split( _str, _delim ).first;
        }

        /* This is what gets called when we write ‹iter->foo›. See
         * ‹proxy› above for a detailed explanation of how and why
         * this works. Also, ‹const› again. */

        proxy operator->() const
        {
            return { **this };
        }

        /* Finally, equality. There is a trap or two that we need to
         * avoid: first and foremost, ‹string_view› comparison
         * operators compare «content» (i.e. the actual strings) –
         * this is not what we want, since it could get really slow,
         * even though it would ‘work’.
         *
         * The other possible trap is that on many implementations,
         * string literals with equal content get equal addresses,
         * i.e. the ‹begin› of two different ‹std::string_view( "" )›
         * instances would compare equal, but this is «not»
         * guaranteed by the standard. It just happens to work by
         * accident on many systems. */

        bool operator==( const iterator &o ) const
        {
            return ( _str.empty() && o._str.empty() ) ||
                   ( _str.begin() == o._str.begin() );
        }
    };

    auto begin() const { return iterator( _str, _delim ); }
    auto end()   const { return iterator( {},   _delim ); }

    splitter( std::string_view str, char delim )
        : _str( str ), _delim( delim )
    {}
};

int main() /* demo */
{
    auto s = splitter( "quick brown fox", ' ' );
    auto e = std::vector{ "quick", "brown", "fox" };

    auto iseq = [&]{ return std::equal( s.begin(), s.end(),
                                        e.begin(), e.end() ); };

    assert( iseq() );

    s = splitter( "", ' ' );
    assert( !iseq() );
    e.clear();
    assert( iseq() );

    s = splitter( "hello", ' ' );
    e = std::vector{ "hello" };
    assert( iseq() );

    s = splitter( "hello", 'l' );
    e = std::vector{ "he", "", "o" };
    assert( iseq() );
}
