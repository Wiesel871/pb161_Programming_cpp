std::uint64_t concat( std::uint64_t a,
                      std::uint64_t b, int b_bits )
{
    return a << b_bits | b;
}
