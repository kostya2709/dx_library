
#ifndef MY_TYPE
#define MY_TYPE 1

template <typename Type>
class super_type
{
public:
    static int assignment_counter;
    static int comparison_counter;

//private:
    Type data;

public:

    super_type (void) noexcept;
    super_type (const Type& object) noexcept;
    super_type (const super_type& object) noexcept;
    super_type& operator= (const super_type& object) noexcept;
    bool operator> (const super_type& object) const noexcept;

    static void reset_counter (void) noexcept;
    static Type get_assignment (void);
    static Type get_comparison (void);
};


template <typename Type>
super_type <Type>::super_type (void) noexcept : data()
{

}

template <typename Type>
super_type <Type>::super_type (const super_type& object) noexcept : data (object.data)
{

}

template <typename Type>
super_type <Type>::super_type (const Type& object) noexcept : data (object)
{

}

template <typename Type>
super_type<Type>& super_type <Type>::operator= (const super_type& object) noexcept
{
    ++assignment_counter;
    data = object.data;
    return *this;
}


template <typename Type>
bool super_type <Type>::operator> (const super_type& object) const noexcept
{
    ++comparison_counter;
    return data > object.data;
}

template <typename Type>
void super_type <Type>::reset_counter (void) noexcept
{
    assignment_counter = 0;
    comparison_counter = 0;
}


template <typename Type>
Type super_type <Type>::get_assignment (void)
{
    return super_type::assignment_counter;
}

template <typename Type>
Type super_type <Type>::get_comparison (void)
{
    return super_type::comparison_counter;
}

#endif