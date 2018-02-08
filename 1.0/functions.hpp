#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP


namespace Functions
{
template<typename T>
static inline T aligned(const T val, const T alignment)
{
    return (val + alignment - 1) & ~(alignment - 1);
}
}

#endif // FUNCTIONS_HPP
