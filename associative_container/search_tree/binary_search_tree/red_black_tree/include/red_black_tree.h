#ifndef MATH_PRACTICE_AND_OPERATING_SYSTEMS_RED_BLACK_TREE_H
#define MATH_PRACTICE_AND_OPERATING_SYSTEMS_RED_BLACK_TREE_H

#include <binary_search_tree.h>

namespace __detail
{
    class RB_TAG;

    template<typename tkey, typename tvalue, typename compare>
    class bst_impl<tkey, tvalue, compare, RB_TAG>
    {
        template<class ...Args>
        static binary_search_tree<tkey, tvalue, compare, RB_TAG>::node* create_node(binary_search_tree<tkey, tvalue, compare, RB_TAG>& cont, Args&& ...args);

        static void delete_node(binary_search_tree<tkey, tvalue, compare, RB_TAG>& cont);

        //Does not invalidate node*, needed for splay tree
        static void post_search(binary_search_tree<tkey, tvalue, compare, RB_TAG>::node**){}

        //Does not invalidate node*
        static void post_insert(binary_search_tree<tkey, tvalue, compare, RB_TAG>& cont, binary_search_tree<tkey, tvalue, compare, RB_TAG>::node**);

        static void erase(binary_search_tree<tkey, tvalue, compare, RB_TAG>& cont, binary_search_tree<tkey, tvalue, compare, RB_TAG>::node**);

        static void swap(binary_search_tree<tkey, tvalue, compare, RB_TAG>& lhs, binary_search_tree<tkey, tvalue, compare, RB_TAG>& rhs) noexcept;
    };
}

template<typename tkey,typename tvalue, compator<tkey> compare = std::less<tkey>>
class red_black_tree final: public binary_search_tree<tkey, tvalue, compare, __detail::RB_TAG>
{

public:
    
    enum class node_color : unsigned char
    {
        RED,
        BLACK
    };

private:

    using parent = binary_search_tree<tkey, tvalue, compare, __detail::RB_TAG>;
    
    struct node final:
        parent::node
    {
        node_color color;

        template<class ...Args>
        node(parent::node* par, Args&&... args);

        ~node() noexcept override =default;
    };


public:

    using value_type = parent::value_type;

    explicit red_black_tree(
            const compare& comp = compare(),
            pp_allocator<value_type> alloc = pp_allocator<value_type>(),
            logger *log = nullptr);

    explicit red_black_tree(
            pp_allocator<value_type> alloc,
            const compare& comp = compare(),
            logger *log = nullptr);

    template<input_iterator_for_pair<tkey, tvalue> iterator>
    explicit red_black_tree(iterator begin, iterator end, const compare& cmp = compare(),
            pp_allocator<value_type> alloc = pp_allocator<value_type>(),
            logger* log = nullptr);

    template<std::ranges::input_range Range>
    explicit red_black_tree(Range&& range, const compare& cmp = compare(),
            pp_allocator<value_type> alloc = pp_allocator<value_type>(),
            logger* log = nullptr);


    red_black_tree(std::initializer_list<std::pair<tkey, tvalue>> data, const compare& cmp = compare(),
            pp_allocator<value_type> alloc = pp_allocator<value_type>(),
            logger* log = nullptr);


    // region iterator definition


    class prefix_iterator : public parent::prefix_iterator
    {
    public:

        using value_type = parent::prefix_iterator::value_type;
        using difference_type = parent::prefix_iterator::difference_type;
        using pointer = parent::prefix_iterator::pointer;
        using reference = parent::prefix_iterator::reference;
        using iterator_category = parent::prefix_iterator::iterator_category;

        explicit prefix_iterator(parent::node* n = nullptr) noexcept;
        prefix_iterator(parent::prefix_iterator) noexcept;

        node_color get_color() const noexcept;

        using parent::prefix_iterator::depth;
        using parent::prefix_iterator::operator*;
        using parent::prefix_iterator::operator==;
        using parent::prefix_iterator::operator!=;
        using parent::prefix_iterator::operator++;
        using parent::prefix_iterator::operator--;
        using parent::prefix_iterator::operator->;
    };

    class prefix_const_iterator : public parent::prefix_const_iterator
    {
    public:

        using value_type = parent::prefix_const_iterator::value_type;
        using difference_type = parent::prefix_const_iterator::difference_type;
        using pointer = parent::prefix_const_iterator::pointer;
        using reference = parent::prefix_const_iterator::reference;
        using iterator_category = parent::prefix_const_iterator::iterator_category;

        explicit prefix_const_iterator(parent::node* n = nullptr) noexcept;
        prefix_const_iterator(parent::prefix_const_iterator) noexcept;

        node_color get_color() const noexcept;

        prefix_const_iterator(prefix_iterator) noexcept;

        using parent::prefix_const_iterator::depth;
        using parent::prefix_const_iterator::operator*;
        using parent::prefix_const_iterator::operator==;
        using parent::prefix_const_iterator::operator!=;
        using parent::prefix_const_iterator::operator++;
        using parent::prefix_const_iterator::operator--;
        using parent::prefix_const_iterator::operator->;
    };

    class prefix_reverse_iterator : public parent::prefix_reverse_iterator
    {
    public:

        using value_type = parent::prefix_reverse_iterator::value_type;
        using difference_type = parent::prefix_reverse_iterator::difference_type;
        using pointer = parent::prefix_reverse_iterator::pointer;
        using reference = parent::prefix_reverse_iterator::reference;
        using iterator_category = parent::prefix_reverse_iterator::iterator_category;

        explicit prefix_reverse_iterator(parent::node* n = nullptr) noexcept;
        prefix_reverse_iterator(parent::prefix_reverse_iterator) noexcept;

        node_color get_color() const noexcept;

        prefix_reverse_iterator(prefix_iterator) noexcept;
        operator prefix_iterator() const noexcept;
        prefix_iterator base() const noexcept;

        using parent::prefix_reverse_iterator::depth;
        using parent::prefix_reverse_iterator::operator*;
        using parent::prefix_reverse_iterator::operator==;
        using parent::prefix_reverse_iterator::operator!=;
        using parent::prefix_reverse_iterator::operator++;
        using parent::prefix_reverse_iterator::operator--;
        using parent::prefix_reverse_iterator::operator->;
    };

    class prefix_const_reverse_iterator : public parent::prefix_const_reverse_iterator
    {
    public:

        using value_type = parent::prefix_const_reverse_iterator::value_type;
        using difference_type = parent::prefix_const_reverse_iterator::difference_type;
        using pointer = parent::prefix_const_reverse_iterator::pointer;
        using reference = parent::prefix_const_reverse_iterator::reference;
        using iterator_category = parent::prefix_const_reverse_iterator::iterator_category;

        explicit prefix_const_reverse_iterator(parent::node* n = nullptr) noexcept;
        prefix_const_reverse_iterator(parent::prefix_const_reverse_iterator) noexcept;

        node_color get_color() const noexcept;

        prefix_const_reverse_iterator(prefix_const_iterator) noexcept;
        operator prefix_const_iterator() const noexcept;
        prefix_const_iterator base() const noexcept;

        using parent::prefix_const_reverse_iterator::depth;
        using parent::prefix_const_reverse_iterator::operator*;
        using parent::prefix_const_reverse_iterator::operator==;
        using parent::prefix_const_reverse_iterator::operator!=;
        using parent::prefix_const_reverse_iterator::operator++;
        using parent::prefix_const_reverse_iterator::operator--;
        using parent::prefix_const_reverse_iterator::operator->;
    };

    class infix_iterator : public parent::infix_iterator
    {
    public:

        using value_type = parent::infix_iterator::value_type;
        using difference_type = parent::infix_iterator::difference_type;
        using pointer = parent::infix_iterator::pointer;
        using reference = parent::infix_iterator::reference;
        using iterator_category = parent::infix_iterator::iterator_category;

        explicit infix_iterator(parent::node* n = nullptr) noexcept;
        infix_iterator(parent::infix_iterator) noexcept;

        node_color get_color() const noexcept;

        using parent::infix_iterator::depth;
        using parent::infix_iterator::operator*;
        using parent::infix_iterator::operator==;
        using parent::infix_iterator::operator!=;
        using parent::infix_iterator::operator++;
        using parent::infix_iterator::operator--;
        using parent::infix_iterator::operator->;
    };

    class infix_const_iterator : parent::infix_const_iterator
    {
    public:

        using value_type = parent::infix_const_iterator::value_type;
        using difference_type = parent::infix_const_iterator::difference_type;
        using pointer = parent::infix_const_iterator::pointer;
        using reference = parent::infix_const_iterator::reference;
        using iterator_category = parent::infix_const_iterator::iterator_category;

        explicit infix_const_iterator(parent::node* n = nullptr) noexcept;
        infix_const_iterator(parent::infix_const_iterator) noexcept;

        node_color get_color() const noexcept;

        infix_const_iterator(infix_iterator) noexcept;

        using parent::infix_const_iterator::depth;
        using parent::infix_const_iterator::operator*;
        using parent::infix_const_iterator::operator==;
        using parent::infix_const_iterator::operator!=;
        using parent::infix_const_iterator::operator++;
        using parent::infix_const_iterator::operator--;
        using parent::infix_const_iterator::operator->;
    };

    class infix_reverse_iterator : public parent::infix_reverse_iterator
    {
    public:

        using value_type = parent::infix_reverse_iterator::value_type;
        using difference_type = parent::infix_reverse_iterator::difference_type;
        using pointer = parent::infix_reverse_iterator::pointer;
        using reference = parent::infix_reverse_iterator::reference;
        using iterator_category = parent::infix_reverse_iterator::iterator_category;

        explicit infix_reverse_iterator(parent::node* n = nullptr) noexcept;
        infix_reverse_iterator(parent::infix_reverse_iterator) noexcept;

        node_color get_color() const noexcept;

        infix_reverse_iterator(infix_iterator) noexcept;
        operator infix_iterator() const noexcept;
        infix_iterator base() const noexcept;

        using parent::infix_reverse_iterator::depth;
        using parent::infix_reverse_iterator::operator*;
        using parent::infix_reverse_iterator::operator==;
        using parent::infix_reverse_iterator::operator!=;
        using parent::infix_reverse_iterator::operator++;
        using parent::infix_reverse_iterator::operator--;
        using parent::infix_reverse_iterator::operator->;
    };

    class infix_const_reverse_iterator : public parent::infix_const_reverse_iterator
    {
    public:

        using value_type = parent::infix_const_reverse_iterator::value_type;
        using difference_type = parent::infix_const_reverse_iterator::difference_type;
        using pointer = parent::infix_const_reverse_iterator::pointer;
        using reference = parent::infix_const_reverse_iterator::reference;
        using iterator_category = parent::infix_const_reverse_iterator::iterator_category;

        explicit infix_const_reverse_iterator(parent::node* n = nullptr) noexcept;
        infix_const_reverse_iterator(parent::infix_const_reverse_iterator) noexcept;

        node_color get_color() const noexcept;

        infix_const_reverse_iterator(infix_const_iterator) noexcept;
        operator infix_const_iterator() const noexcept;
        infix_const_iterator base() const noexcept;

        using parent::infix_const_reverse_iterator::depth;
        using parent::infix_const_reverse_iterator::operator*;
        using parent::infix_const_reverse_iterator::operator==;
        using parent::infix_const_reverse_iterator::operator!=;
        using parent::infix_const_reverse_iterator::operator++;
        using parent::infix_const_reverse_iterator::operator--;
        using parent::infix_const_reverse_iterator::operator->;
    };

    class postfix_iterator : public parent::postfix_iterator
    {
    public:

        using value_type = parent::postfix_iterator::value_type;
        using difference_type = parent::postfix_iterator::difference_type;
        using pointer = parent::postfix_iterator::pointer;
        using reference = parent::postfix_iterator::reference;
        using iterator_category = parent::postfix_iterator::iterator_category;

        explicit postfix_iterator(parent::node* n = nullptr) noexcept;
        postfix_iterator(parent::postfix_iterator) noexcept;

        node_color get_color() const noexcept;

        using parent::postfix_iterator::depth;
        using parent::postfix_iterator::operator*;
        using parent::postfix_iterator::operator==;
        using parent::postfix_iterator::operator!=;
        using parent::postfix_iterator::operator++;
        using parent::postfix_iterator::operator--;
        using parent::postfix_iterator::operator->;
    };

    class postfix_const_iterator : public parent::postfix_const_iterator
    {
    public:

        using value_type = parent::postfix_const_iterator::value_type;
        using difference_type = parent::postfix_const_iterator::difference_type;
        using pointer = parent::postfix_const_iterator::pointer;
        using reference = parent::postfix_const_iterator::reference;
        using iterator_category = parent::postfix_const_iterator::iterator_category;

        explicit postfix_const_iterator(parent::node* n = nullptr) noexcept;
        postfix_const_iterator(parent::postfix_const_iterator) noexcept;

        node_color get_color() const noexcept;

        postfix_const_iterator(postfix_iterator) noexcept;

        using parent::postfix_const_iterator::depth;
        using parent::postfix_const_iterator::operator*;
        using parent::postfix_const_iterator::operator==;
        using parent::postfix_const_iterator::operator!=;
        using parent::postfix_const_iterator::operator++;
        using parent::postfix_const_iterator::operator--;
        using parent::postfix_const_iterator::operator->;
    };

    class postfix_reverse_iterator : public parent::postfix_reverse_iterator
    {
    public:

        using value_type = parent::postfix_reverse_iterator::value_type;
        using difference_type = parent::postfix_reverse_iterator::difference_type;
        using pointer = parent::postfix_reverse_iterator::pointer;
        using reference = parent::postfix_reverse_iterator::reference;
        using iterator_category = parent::postfix_reverse_iterator::iterator_category;

        explicit postfix_reverse_iterator(parent::node* n = nullptr) noexcept;
        postfix_reverse_iterator(parent::postfix_reverse_iterator) noexcept;

        node_color get_color() const noexcept;

        postfix_reverse_iterator(postfix_iterator) noexcept;
        operator postfix_iterator() const noexcept;
        postfix_iterator base() const noexcept;

        using parent::postfix_reverse_iterator::depth;
        using parent::postfix_reverse_iterator::operator*;
        using parent::postfix_reverse_iterator::operator==;
        using parent::postfix_reverse_iterator::operator!=;
        using parent::postfix_reverse_iterator::operator++;
        using parent::postfix_reverse_iterator::operator--;
        using parent::postfix_reverse_iterator::operator->;
    };

    class postfix_const_reverse_iterator : public parent::postfix_const_reverse_iterator
    {
    public:

        using value_type = parent::postfix_const_reverse_iterator::value_type;
        using difference_type = parent::postfix_const_reverse_iterator::difference_type;
        using pointer = parent::postfix_const_reverse_iterator::pointer;
        using reference = parent::postfix_const_reverse_iterator::reference;
        using iterator_category = parent::postfix_const_reverse_iterator::iterator_category;

        explicit postfix_const_reverse_iterator(parent::node* n = nullptr) noexcept;
        postfix_const_reverse_iterator(parent::postfix_const_reverse_iterator) noexcept;

        node_color get_color() const noexcept;

        postfix_const_reverse_iterator(postfix_const_iterator) noexcept;
        operator postfix_const_iterator() const noexcept;
        postfix_const_iterator base() const noexcept;

        using parent::postfix_const_reverse_iterator::depth;
        using parent::postfix_const_reverse_iterator::operator*;
        using parent::postfix_const_reverse_iterator::operator==;
        using parent::postfix_const_reverse_iterator::operator!=;
        using parent::postfix_const_reverse_iterator::operator++;
        using parent::postfix_const_reverse_iterator::operator--;
        using parent::postfix_const_reverse_iterator::operator->;

    };



    // endregion iterator definition
    
    // region iterator requests declaration

    infix_iterator begin() noexcept;

    infix_iterator end() noexcept;

    infix_const_iterator begin() const noexcept;

    infix_const_iterator end() const noexcept;

    infix_const_iterator cbegin() const noexcept;

    infix_const_iterator cend() const noexcept;

    infix_reverse_iterator rbegin() noexcept;

    infix_reverse_iterator rend() noexcept;

    infix_const_reverse_iterator rbegin() const noexcept;

    infix_const_reverse_iterator rend() const noexcept;

    infix_const_reverse_iterator crbegin() const noexcept;

    infix_const_reverse_iterator crend() const noexcept;


    prefix_iterator begin_prefix() noexcept;

    prefix_iterator end_prefix() noexcept;

    prefix_const_iterator begin_prefix() const noexcept;

    prefix_const_iterator end_prefix() const noexcept;

    prefix_const_iterator cbegin_prefix() const noexcept;

    prefix_const_iterator cend_prefix() const noexcept;

    prefix_reverse_iterator rbegin_prefix() noexcept;

    prefix_reverse_iterator rend_prefix() noexcept;

    prefix_const_reverse_iterator rbegin_prefix() const noexcept;

    prefix_const_reverse_iterator rend_prefix() const noexcept;

    prefix_const_reverse_iterator crbegin_prefix() const noexcept;

    prefix_const_reverse_iterator crend_prefix() const noexcept;


    infix_iterator begin_infix() noexcept;

    infix_iterator end_infix() noexcept;

    infix_const_iterator begin_infix() const noexcept;

    infix_const_iterator end_infix() const noexcept;

    infix_const_iterator cbegin_infix() const noexcept;

    infix_const_iterator cend_infix() const noexcept;

    infix_reverse_iterator rbegin_infix() noexcept;

    infix_reverse_iterator rend_infix() noexcept;

    infix_const_reverse_iterator rbegin_infix() const noexcept;

    infix_const_reverse_iterator rend_infix() const noexcept;

    infix_const_reverse_iterator crbegin_infix() const noexcept;

    infix_const_reverse_iterator crend_infix() const noexcept;


    postfix_iterator begin_postfix() noexcept;

    postfix_iterator end_postfix() noexcept;

    postfix_const_iterator begin_postfix() const noexcept;

    postfix_const_iterator end_postfix() const noexcept;

    postfix_const_iterator cbegin_postfix() const noexcept;

    postfix_const_iterator cend_postfix() const noexcept;

    postfix_reverse_iterator rbegin_postfix() noexcept;

    postfix_reverse_iterator rend_postfix() noexcept;

    postfix_const_reverse_iterator rbegin_postfix() const noexcept;

    postfix_const_reverse_iterator rend_postfix() const noexcept;

    postfix_const_reverse_iterator crbegin_postfix() const noexcept;

    postfix_const_reverse_iterator crend_postfix() const noexcept;

    // endregion iterator requests declaration
    
public:
    
    ~red_black_tree() noexcept final;
    
    red_black_tree(red_black_tree const &other);
    
    red_black_tree &operator=(red_black_tree const &other);
    
    red_black_tree(red_black_tree &&other) noexcept;
    
    red_black_tree &operator=(red_black_tree &&other) noexcept;


    void swap(parent& other) noexcept override;


    /** Only rebinds iterators
     */
    std::pair<infix_iterator, bool> insert(const value_type&);
    std::pair<infix_iterator, bool> insert(value_type&&);

    template<class ...Args>
    std::pair<infix_iterator, bool> emplace(Args&&...args);

    infix_iterator insert_or_assign(const value_type&);
    infix_iterator insert_or_assign(value_type&&);

    template<class ...Args>
    infix_iterator emplace_or_assign(Args&&...args);

    infix_iterator find(const tkey&);
    infix_const_iterator find(const tkey&) const;

    infix_iterator lower_bound(const tkey&);
    infix_const_iterator lower_bound(const tkey&) const;

    infix_iterator upper_bound(const tkey&);
    infix_const_iterator upper_bound(const tkey&) const;

    infix_iterator erase(infix_iterator pos);
    infix_iterator erase(infix_const_iterator pos);

    infix_iterator erase(infix_iterator first, infix_iterator last);
    infix_iterator erase(infix_const_iterator first, infix_const_iterator last);

    using parent::erase;
    using parent::insert;
    using parent::insert_or_assign;
};

template<typename compare, typename U, typename iterator>
explicit red_black_tree(iterator begin, iterator end, const compare& cmp = compare(),
        pp_allocator<U> alloc = pp_allocator<U>(),
        logger* log = nullptr) -> red_black_tree<typename std::iterator_traits<iterator>::value_type::first_type, typename std::iterator_traits<iterator>::value_type::second_type, compare>;

template<typename compare, typename U, std::ranges::forward_range Range>
explicit red_black_tree(Range&& range, const compare& cmp = compare(),
        pp_allocator<U> alloc = pp_allocator<U>(),
        logger* log = nullptr) -> red_black_tree<typename std::iterator_traits<typename std::ranges::iterator_t<Range>>::value_type::first_type, typename std::iterator_traits<typename std::ranges::iterator_t<Range>>::value_type::second_type, compare> ;

template<typename tkey, typename tvalue, typename compare, typename U>
red_black_tree(std::initializer_list<std::pair<tkey, tvalue>> data, const compare& cmp = compare(),
        pp_allocator<U> alloc = pp_allocator<U>(),
        logger* log = nullptr) -> red_black_tree<tkey, tvalue, compare>;

namespace __detail {

    class RB_TAG {};

    template<typename tkey, typename tvalue, typename compare>
    template<class ...Args>
    binary_search_tree<tkey, tvalue, compare, RB_TAG>::node* bst_impl<tkey, tvalue, compare, RB_TAG>::create_node(
            binary_search_tree<tkey, tvalue, compare, RB_TAG>& cont, Args&& ...args)
    {
        throw not_implemented("template<typename tkey, typename tvalue, typename compare> template<class ...Args> binary_search_tree<tkey, tvalue, compare, RB_TAG>::node* bst_impl<tkey, tvalue, compare, RB_TAG>::create_node(binary_search_tree<tkey, tvalue, compare, RB_TAG>& cont, Args&& ...args)", "your code should be here...");
    }

    template<typename tkey, typename tvalue, typename compare>
    void bst_impl<tkey, tvalue, compare, RB_TAG>::delete_node(
            binary_search_tree<tkey, tvalue, compare, RB_TAG>& cont)
    {
        throw not_implemented("template<typename tkey, typename tvalue, typename compare> void bst_impl<tkey, tvalue, compare, RB_TAG>::delete_node(binary_search_tree<tkey, tvalue, compare, RB_TAG>& cont)", "your code should be here...");
    }

    template<typename tkey, typename tvalue, typename compare>
    void bst_impl<tkey, tvalue, compare, RB_TAG>::post_insert(
            binary_search_tree<tkey, tvalue, compare, RB_TAG>& cont,
            typename binary_search_tree<tkey, tvalue, compare, RB_TAG>::node** n)
    {
        throw not_implemented("template<typename tkey, typename tvalue, typename compare> void bst_impl<tkey, tvalue, compare, RB_TAG>::post_insert(binary_search_tree<tkey, tvalue, compare, RB_TAG>& cont, typename binary_search_tree<tkey, tvalue, compare, RB_TAG>::node**)", "your code should be here...");
    }

    template<typename tkey, typename tvalue, typename compare>
    void bst_impl<tkey, tvalue, compare, RB_TAG>::erase(
            binary_search_tree<tkey, tvalue, compare, RB_TAG>& cont,
            typename binary_search_tree<tkey, tvalue, compare, RB_TAG>::node** n)
    {
        throw not_implemented("template<typename tkey, typename tvalue, typename compare> void bst_impl<tkey, tvalue, compare, RB_TAG>::erase(binary_search_tree<tkey, tvalue, compare, RB_TAG>& cont, typename binary_search_tree<tkey, tvalue, compare, RB_TAG>::node**)", "your code should be here...");
    }

    template<typename tkey, typename tvalue, typename compare>
    void bst_impl<tkey, tvalue, compare, RB_TAG>::swap(binary_search_tree<tkey, tvalue, compare, RB_TAG> &lhs,
                                                                            binary_search_tree<tkey, tvalue, compare, RB_TAG> &rhs) noexcept
    {
        throw not_implemented("template<typename tkey, typename tvalue, typename compare>\n"
                              "void bst_impl<tkey, tvalue, compare, RB_TAG>::swap(binary_search_tree<tkey, tvalue, compare, RB_TAG> &lhs,\n"
                              "binary_search_tree<tkey, tvalue, compare, RB_TAG> &rhs) noexcept", "your code should be here...");
    }
}


template<typename tkey, typename tvalue, compator<tkey> compare>
template<class ...Args>
red_black_tree<tkey, tvalue, compare>::node::node(parent::node* par, Args&&... args)
{
    throw not_implemented("template<typename tkey, typename tvalue, compator<tkey> compare> template<class ...Args> red_black_tree<tkey, tvalue, compare>::node::node(parent::node* par, Args&&... args)", "your code should be here...");
}

template<typename tkey, typename tvalue, compator<tkey> compare>
red_black_tree<tkey, tvalue, compare>::red_black_tree(
        const compare& comp,
        pp_allocator<value_type> alloc,
        logger *log)
{
    throw not_implemented("template<typename tkey, typename tvalue, compator<tkey> compare> red_black_tree<tkey, tvalue, compare>::red_black_tree(const compare& , pp_allocator<value_type> , logger *)", "your code should be here...");
}

template<typename tkey, typename tvalue, compator<tkey> compare>
red_black_tree<tkey, tvalue, compare>::red_black_tree(
        pp_allocator<value_type> alloc,
        const compare& comp,
        logger *log)
{
    throw not_implemented("template<typename tkey, typename tvalue, compator<tkey> compare> red_black_tree<tkey, tvalue, compare>::red_black_tree(pp_allocator<value_type> , const compare& , logger *)", "your code should be here...");
}

template<typename tkey, typename tvalue, compator<tkey> compare>
template<input_iterator_for_pair<tkey, tvalue> iterator>
red_black_tree<tkey, tvalue, compare>::red_black_tree(
        iterator begin, iterator end,
        const compare& cmp,
        pp_allocator<value_type> alloc,
        logger* log)
{
    throw not_implemented("template<typename tkey, typename tvalue, compator<tkey> compare> template<input_iterator_for_pair<tkey, tvalue> iterator> red_black_tree<tkey, tvalue, compare>::red_black_tree(iterator , iterator , const compare& , pp_allocator<value_type> , logger* )", "your code should be here...");
}

template<typename tkey, typename tvalue, compator<tkey> compare>
template<std::ranges::input_range Range>
red_black_tree<tkey, tvalue, compare>::red_black_tree(
        Range&& range,
        const compare& cmp,
        pp_allocator<value_type> alloc,
        logger* log)
{
    throw not_implemented("template<typename tkey, typename tvalue, compator<tkey> compare> template<std::ranges::input_range Range> red_black_tree<tkey, tvalue, compare>::red_black_tree( Range&& range, const compare& cmp, pp_allocator<value_type> alloc, logger* log)", "your code should be here...");
}

template<typename tkey, typename tvalue, compator<tkey> compare>
red_black_tree<tkey, tvalue, compare>::red_black_tree(
        std::initializer_list<std::pair<tkey, tvalue>> data,
        const compare& cmp,
        pp_allocator<value_type> alloc,
        logger* log)
{
    throw not_implemented("template<typename tkey, typename tvalue, compator<tkey> compare> red_black_tree<tkey, tvalue, compare>::red_black_tree(std::initializer_list<std::pair<tkey, tvalue>> data, const compare& cmp, pp_allocator<value_type> alloc, logger* log)", "your code should be here...");
}

// region iterator implementation

template<typename tkey, typename tvalue, compator<tkey> compare>
red_black_tree<tkey, tvalue, compare>::prefix_iterator::prefix_iterator(parent::node* n) noexcept
{
    throw not_implemented("template<typename tkey, typename tvalue, typename compare> red_black_tree<tkey, tvalue, compare>::prefix_iterator::prefix_iterator(parent::node* n) noexcept", "your code should be here...");
}

template<typename tkey, typename tvalue, compator<tkey> compare>
red_black_tree<tkey, tvalue, compare>::prefix_iterator::prefix_iterator(parent::prefix_iterator it) noexcept
{
    throw not_implemented("template<typename tkey, typename tvalue, compator<tkey> compare> red_black_tree<tkey, tvalue, compare>::prefix_iterator::prefix_iterator(parent::prefix_iterator it) noexcept", "your code should be here...");
}

template<typename tkey, typename tvalue, compator<tkey> compare>
typename red_black_tree<tkey, tvalue, compare>::node_color
red_black_tree<tkey, tvalue, compare>::prefix_iterator::get_color() const noexcept
{
    throw not_implemented("template<typename tkey, typename tvalue, typename compare> typename red_black_tree<tkey, tvalue, compare>::node_color red_black_tree<tkey, tvalue, compare>::prefix_iterator::get_color() const noexcept ", "your code should be here...");
}

template<typename tkey, typename tvalue, compator<tkey> compare>
red_black_tree<tkey, tvalue, compare>::prefix_const_iterator::prefix_const_iterator(parent::node* n) noexcept
{
    throw not_implemented("template<typename tkey, typename tvalue, typename compare> red_black_tree<tkey, tvalue, compare>::prefix_const_iterator::prefix_const_iterator(parent::node* n) noexcept", "your code should be here...");
}

template<typename tkey, typename tvalue, compator<tkey> compare>
red_black_tree<tkey, tvalue, compare>::prefix_const_iterator::prefix_const_iterator(parent::prefix_const_iterator it) noexcept
{
    throw not_implemented("template<typename tkey, typename tvalue, typename compare> red_black_tree<tkey, tvalue, compare>::prefix_const_iterator::prefix_const_iterator(parent::prefix_const_iterator it) noexcept", "your code should be here...");
}

template<typename tkey, typename tvalue, compator<tkey> compare>
typename red_black_tree<tkey, tvalue, compare>::node_color
red_black_tree<tkey, tvalue, compare>::prefix_const_iterator::get_color() const noexcept
{
    throw not_implemented("template<typename tkey, typename tvalue, typename compare> typename red_black_tree<tkey, tvalue, compare>::node_color red_black_tree<tkey, tvalue, compare>::prefix_const_iterator::get_color() const noexcept ", "your code should be here...");
}

template<typename tkey, typename tvalue, compator<tkey> compare>
red_black_tree<tkey, tvalue, compare>::prefix_const_iterator::prefix_const_iterator(prefix_iterator it) noexcept
{
    throw not_implemented("template<typename tkey, typename tvalue, typename compare> red_black_tree<tkey, tvalue, compare>::prefix_const_iterator::prefix_const_iterator(prefix_iterator it) noexcept", "your code should be here...");
}

template<typename tkey, typename tvalue, compator<tkey> compare>
red_black_tree<tkey, tvalue, compare>::prefix_reverse_iterator::prefix_reverse_iterator(parent::node* n) noexcept
{
    throw not_implemented("template<typename tkey, typename tvalue, typename compare> red_black_tree<tkey, tvalue, compare>::prefix_reverse_iterator::prefix_reverse_iterator(parent::node* n) noexcept", "your code should be here...");
}

template<typename tkey, typename tvalue, compator<tkey> compare>
red_black_tree<tkey, tvalue, compare>::prefix_reverse_iterator::prefix_reverse_iterator(parent::prefix_reverse_iterator it) noexcept
{
    throw not_implemented("template<typename tkey, typename tvalue, typename compare> red_black_tree<tkey, tvalue, compare>::prefix_reverse_iterator::prefix_reverse_iterator(parent::prefix_reverse_iterator it) noexcept", "your code should be here...");
}

template<typename tkey, typename tvalue, compator<tkey> compare>
typename red_black_tree<tkey, tvalue, compare>::node_color
red_black_tree<tkey, tvalue, compare>::prefix_reverse_iterator::get_color() const noexcept
{
    throw not_implemented("template<typename tkey, typename tvalue, typename compare> typename red_black_tree<tkey, tvalue, compare>::node_color red_black_tree<tkey, tvalue, compare>::prefix_reverse_iterator::get_color() const noexcept ", "your code should be here...");
}

template<typename tkey, typename tvalue, compator<tkey> compare>
red_black_tree<tkey, tvalue, compare>::prefix_reverse_iterator::prefix_reverse_iterator(prefix_iterator it) noexcept
{
    throw not_implemented("template<typename tkey, typename tvalue, typename compare> red_black_tree<tkey, tvalue, compare>::prefix_reverse_iterator::prefix_reverse_iterator(prefix_iterator it) noexcept", "your code should be here...");
}

template<typename tkey, typename tvalue, compator<tkey> compare>
red_black_tree<tkey, tvalue, compare>::prefix_reverse_iterator::operator red_black_tree<tkey, tvalue, compare>::prefix_iterator() const noexcept
{
    throw not_implemented("template<typename tkey, typename tvalue, typename compare> red_black_tree<tkey, tvalue, compare>::prefix_reverse_iterator::operator prefix_iterator() const noexcept ", "your code should be here...");
}

template<typename tkey, typename tvalue, compator<tkey> compare>
typename red_black_tree<tkey, tvalue, compare>::prefix_iterator
red_black_tree<tkey, tvalue, compare>::prefix_reverse_iterator::base() const noexcept
{
    throw not_implemented("template<typename tkey, typename tvalue, typename compare> typename red_black_tree<tkey, tvalue, compare>::prefix_iterator red_black_tree<tkey, tvalue, compare>::prefix_reverse_iterator::base() const noexcept ", "your code should be here...");
}

template<typename tkey, typename tvalue, compator<tkey> compare>
red_black_tree<tkey, tvalue, compare>::prefix_const_reverse_iterator::prefix_const_reverse_iterator(parent::node* n) noexcept
{
    throw not_implemented("template<typename tkey, typename tvalue, typename compare> red_black_tree<tkey, tvalue, compare>::prefix_const_reverse_iterator::prefix_const_reverse_iterator(parent::node* n) noexcept", "your code should be here...");
}

template<typename tkey, typename tvalue, compator<tkey> compare>
red_black_tree<tkey, tvalue, compare>::prefix_const_reverse_iterator::prefix_const_reverse_iterator(parent::prefix_const_reverse_iterator it) noexcept
{
    throw not_implemented("template<typename tkey, typename tvalue, typename compare> red_black_tree<tkey, tvalue, compare>::prefix_const_reverse_iterator::prefix_const_reverse_iterator(parent::prefix_const_reverse_iterator it) noexcept", "your code should be here...");
}

template<typename tkey, typename tvalue, compator<tkey> compare>
typename red_black_tree<tkey, tvalue, compare>::node_color
red_black_tree<tkey, tvalue, compare>::prefix_const_reverse_iterator::get_color() const noexcept
{
    throw not_implemented("template<typename tkey, typename tvalue, typename compare> typename red_black_tree<tkey, tvalue, compare>::node_color red_black_tree<tkey, tvalue, compare>::prefix_const_reverse_iterator::get_color() const noexcept ", "your code should be here...");
}

template<typename tkey, typename tvalue, compator<tkey> compare>
red_black_tree<tkey, tvalue, compare>::prefix_const_reverse_iterator::prefix_const_reverse_iterator(prefix_const_iterator it) noexcept
{
    throw not_implemented("template<typename tkey, typename tvalue, typename compare> red_black_tree<tkey, tvalue, compare>::prefix_const_reverse_iterator::prefix_const_reverse_iterator(prefix_const_iterator it) noexcept", "your code should be here...");
}

template<typename tkey, typename tvalue, compator<tkey> compare>
red_black_tree<tkey, tvalue, compare>::prefix_const_reverse_iterator::operator red_black_tree<tkey, tvalue, compare>::prefix_const_iterator() const noexcept
{
    throw not_implemented("template<typename tkey, typename tvalue, typename compare> red_black_tree<tkey, tvalue, compare>::prefix_const_reverse_iterator::operator prefix_const_iterator() const noexcept ", "your code should be here...");
}

template<typename tkey, typename tvalue, compator<tkey> compare>
typename red_black_tree<tkey, tvalue, compare>::prefix_const_iterator
red_black_tree<tkey, tvalue, compare>::prefix_const_reverse_iterator::base() const noexcept
{
    throw not_implemented("template<typename tkey, typename tvalue, typename compare> typename red_black_tree<tkey, tvalue, compare>::prefix_const_iterator red_black_tree<tkey, tvalue, compare>::prefix_const_reverse_iterator::base() const noexcept ", "your code should be here...");
}

template<typename tkey, typename tvalue, compator<tkey> compare>
red_black_tree<tkey, tvalue, compare>::infix_iterator::infix_iterator(parent::node* n) noexcept
{
    throw not_implemented("template<typename tkey, typename tvalue, typename compare> red_black_tree<tkey, tvalue, compare>::infix_iterator::infix_iterator(parent::node* n) noexcept", "your code should be here...");
}

template<typename tkey, typename tvalue, compator<tkey> compare>
red_black_tree<tkey, tvalue, compare>::infix_iterator::infix_iterator(parent::infix_iterator it) noexcept
{
    throw not_implemented("template<typename tkey, typename tvalue, typename compare> red_black_tree<tkey, tvalue, compare>::infix_iterator::infix_iterator(parent::infix_iterator it) noexcept", "your code should be here...");
}

template<typename tkey, typename tvalue, compator<tkey> compare>
typename red_black_tree<tkey, tvalue, compare>::node_color
red_black_tree<tkey, tvalue, compare>::infix_iterator::get_color() const noexcept
{
    throw not_implemented("template<typename tkey, typename tvalue, typename compare> typename red_black_tree<tkey, tvalue, compare>::node_color red_black_tree<tkey, tvalue, compare>::infix_iterator::get_color() const noexcept ", "your code should be here...");
}

template<typename tkey, typename tvalue, compator<tkey> compare>
red_black_tree<tkey, tvalue, compare>::infix_const_iterator::infix_const_iterator(parent::node* n) noexcept
{
    throw not_implemented("template<typename tkey, typename tvalue, typename compare> red_black_tree<tkey, tvalue, compare>::infix_const_iterator::infix_const_iterator(parent::node* n) noexcept", "your code should be here...");
}

template<typename tkey, typename tvalue, compator<tkey> compare>
red_black_tree<tkey, tvalue, compare>::infix_const_iterator::infix_const_iterator(parent::infix_const_iterator it) noexcept
{
throw not_implemented("template<typename tkey, typename tvalue, typename compare> red_black_tree<tkey, tvalue, compare>::infix_const_iterator::infix_const_iterator(parent::infix_const_iterator it) noexcept", "your code should be here...");
}

template<typename tkey, typename tvalue, compator<tkey> compare>
typename red_black_tree<tkey, tvalue, compare>::node_color
red_black_tree<tkey, tvalue, compare>::infix_const_iterator::get_color() const noexcept
{
throw not_implemented("template<typename tkey, typename tvalue, typename compare> typename red_black_tree<tkey, tvalue, compare>::node_color red_black_tree<tkey, tvalue, compare>::infix_const_iterator::get_color() const noexcept", "your code should be here...");
}

template<typename tkey, typename tvalue, compator<tkey> compare>
red_black_tree<tkey, tvalue, compare>::infix_const_iterator::infix_const_iterator(infix_iterator it) noexcept
{
throw not_implemented("template<typename tkey, typename tvalue, typename compare> red_black_tree<tkey, tvalue, compare>::infix_const_iterator::infix_const_iterator(infix_iterator it) noexcept", "your code should be here...");
}

template<typename tkey, typename tvalue, compator<tkey> compare>
red_black_tree<tkey, tvalue, compare>::infix_reverse_iterator::infix_reverse_iterator(parent::node* n) noexcept
{
throw not_implemented("template<typename tkey, typename tvalue, typename compare> red_black_tree<tkey, tvalue, compare>::infix_reverse_iterator::infix_reverse_iterator(parent::node* n) noexcept) noexcept", "your code should be here...");
}

template<typename tkey, typename tvalue, compator<tkey> compare>
red_black_tree<tkey, tvalue, compare>::infix_reverse_iterator::infix_reverse_iterator(parent::infix_reverse_iterator it) noexcept
{
throw not_implemented("template<typename tkey, typename tvalue, typename compare> red_black_tree<tkey, tvalue, compare>::infix_reverse_iterator::infix_reverse_iterator(parent::infix_reverse_iterator it) noexcept", "your code should be here...");
}

template<typename tkey, typename tvalue, compator<tkey> compare>
typename red_black_tree<tkey, tvalue, compare>::node_color
red_black_tree<tkey, tvalue, compare>::infix_reverse_iterator::get_color() const noexcept
{
throw not_implemented("template<typename tkey, typename tvalue, typename compare> typename red_black_tree<tkey, tvalue, compare>::node_color red_black_tree<tkey, tvalue, compare>::infix_reverse_iterator::get_color() const noexcept", "your code should be here...");
}

template<typename tkey, typename tvalue, compator<tkey> compare>
red_black_tree<tkey, tvalue, compare>::infix_reverse_iterator::infix_reverse_iterator(infix_iterator it) noexcept
{
throw not_implemented("template<typename tkey, typename tvalue, typename compare> red_black_tree<tkey, tvalue, compare>::infix_reverse_iterator::infix_reverse_iterator(infix_iterator it) noexcept", "your code should be here...");
}

template<typename tkey, typename tvalue, compator<tkey> compare>
red_black_tree<tkey, tvalue, compare>::infix_reverse_iterator::operator red_black_tree<tkey, tvalue, compare>::infix_iterator() const noexcept
{
throw not_implemented("template<typename tkey, typename tvalue, typename compare> red_black_tree<tkey, tvalue, compare>::infix_reverse_iterator::operator infix_iterator() const noexcept", "your code should be here...");
}

template<typename tkey, typename tvalue, compator<tkey> compare>
typename red_black_tree<tkey, tvalue, compare>::infix_iterator
red_black_tree<tkey, tvalue, compare>::infix_reverse_iterator::base() const noexcept
{
throw not_implemented("template<typename tkey, typename tvalue, typename compare> typename red_black_tree<tkey, tvalue, compare>::infix_iterator red_black_tree<tkey, tvalue, compare>::infix_reverse_iterator::base() const noexcept", "your code should be here...");
}

template<typename tkey, typename tvalue, compator<tkey> compare>
red_black_tree<tkey, tvalue, compare>::infix_const_reverse_iterator::infix_const_reverse_iterator(parent::node* n) noexcept
{
throw not_implemented("template<typename tkey, typename tvalue, typename compare> red_black_tree<tkey, tvalue, compare>::infix_const_reverse_iterator::infix_const_reverse_iterator(parent::node* n) noexcept", "your code should be here...");
}

template<typename tkey, typename tvalue, compator<tkey> compare>
red_black_tree<tkey, tvalue, compare>::infix_const_reverse_iterator::infix_const_reverse_iterator(parent::infix_const_reverse_iterator it) noexcept
{
throw not_implemented("template<typename tkey, typename tvalue, typename compare> red_black_tree<tkey, tvalue, compare>::infix_const_reverse_iterator::infix_const_reverse_iterator(parent::infix_const_reverse_iterator it) noexcept", "your code should be here...");
}

template<typename tkey, typename tvalue, compator<tkey> compare>
typename red_black_tree<tkey, tvalue, compare>::node_color
red_black_tree<tkey, tvalue, compare>::infix_const_reverse_iterator::get_color() const noexcept
{
throw not_implemented("template<typename tkey, typename tvalue, typename compare> typename red_black_tree<tkey, tvalue, compare>::node_color red_black_tree<tkey, tvalue, compare>::infix_const_reverse_iterator::get_color() const noexcept", "your code should be here...");
}

template<typename tkey, typename tvalue, compator<tkey> compare>
red_black_tree<tkey, tvalue, compare>::infix_const_reverse_iterator::infix_const_reverse_iterator(infix_const_iterator it) noexcept
{
throw not_implemented("template<typename tkey, typename tvalue, typename compare> red_black_tree<tkey, tvalue, compare>::infix_const_reverse_iterator::infix_const_reverse_iterator(infix_const_iterator it) noexcept", "your code should be here...");
}

template<typename tkey, typename tvalue, compator<tkey> compare>
red_black_tree<tkey, tvalue, compare>::infix_const_reverse_iterator::operator red_black_tree<tkey, tvalue, compare>::infix_const_iterator() const noexcept
{
    throw not_implemented("template<typename tkey, typename tvalue, typename compare> red_black_tree<tkey, tvalue, compare>::infix_const_reverse_iterator::operator infix_const_iterator() const noexcept", "your code should be here...");
}

template<typename tkey, typename tvalue, compator<tkey> compare>
typename red_black_tree<tkey, tvalue, compare>::infix_const_iterator
red_black_tree<tkey, tvalue, compare>::infix_const_reverse_iterator::base() const noexcept
{
    throw not_implemented("template<typename tkey, typename tvalue, typename compare> typename red_black_tree<tkey, tvalue, compare>::infix_const_iterator red_black_tree<tkey, tvalue, compare>::infix_const_reverse_iterator::base() const noexcept", "your code should be here...");
}

template<typename tkey, typename tvalue, compator<tkey> compare>
red_black_tree<tkey, tvalue, compare>::postfix_iterator::postfix_iterator(parent::node* n) noexcept
{
    throw not_implemented("template<typename tkey, typename tvalue, typename compare> red_black_tree<tkey, tvalue, compare>::postfix_iterator::postfix_iterator(parent::node* n) noexcept", "your code should be here...");
}

template<typename tkey, typename tvalue, compator<tkey> compare>
red_black_tree<tkey, tvalue, compare>::postfix_iterator::postfix_iterator(parent::postfix_iterator it) noexcept
{
    throw not_implemented("template<typename tkey, typename tvalue, typename compare> red_black_tree<tkey, tvalue, compare>::postfix_iterator::postfix_iterator(parent::postfix_iterator it) noexcept", "your code should be here...");
}

template<typename tkey, typename tvalue, compator<tkey> compare>
typename red_black_tree<tkey, tvalue, compare>::node_color
red_black_tree<tkey, tvalue, compare>::postfix_iterator::get_color() const noexcept
{
    throw not_implemented("template<typename tkey, typename tvalue, typename compare> typename red_black_tree<tkey, tvalue, compare>::node_color red_black_tree<tkey, tvalue, compare>::postfix_iterator::get_color() const noexcept", "your code should be here...");
}

template<typename tkey, typename tvalue, compator<tkey> compare>
red_black_tree<tkey, tvalue, compare>::postfix_const_iterator::postfix_const_iterator(parent::node* n) noexcept
{
    throw not_implemented("template<typename tkey, typename tvalue, typename compare> red_black_tree<tkey, tvalue, compare>::postfix_const_iterator::postfix_const_iterator(parent::node* n) noexcept", "your code should be here...");
}

template<typename tkey, typename tvalue, compator<tkey> compare>
red_black_tree<tkey, tvalue, compare>::postfix_const_iterator::postfix_const_iterator(parent::postfix_const_iterator it) noexcept
{
    throw not_implemented("template<typename tkey, typename tvalue, typename compare> red_black_tree<tkey, tvalue, compare>::postfix_const_iterator::postfix_const_iterator(parent::postfix_const_iterator it) noexcept", "your code should be here...");
}

template<typename tkey, typename tvalue, compator<tkey> compare>
typename red_black_tree<tkey, tvalue, compare>::node_color
red_black_tree<tkey, tvalue, compare>::postfix_const_iterator::get_color() const noexcept
{
    throw not_implemented("template<typename tkey, typename tvalue, typename compare> typename red_black_tree<tkey, tvalue, compare>::node_color red_black_tree<tkey, tvalue, compare>::postfix_const_iterator::get_color() const noexcept", "your code should be here...");
}

template<typename tkey, typename tvalue, compator<tkey> compare>
red_black_tree<tkey, tvalue, compare>::postfix_const_iterator::postfix_const_iterator(postfix_iterator it) noexcept
{
    throw not_implemented("template<typename tkey, typename tvalue, typename compare> red_black_tree<tkey, tvalue, compare>::postfix_const_iterator::postfix_const_iterator(postfix_iterator it) noexcept", "your code should be here...");
}

template<typename tkey, typename tvalue, compator<tkey> compare>
red_black_tree<tkey, tvalue, compare>::postfix_reverse_iterator::postfix_reverse_iterator(parent::node* n) noexcept
{
    throw not_implemented("template<typename tkey, typename tvalue, typename compare> red_black_tree<tkey, tvalue, compare>::postfix_reverse_iterator::postfix_reverse_iterator(parent::node* n) noexcept", "your code should be here...");
}

template<typename tkey, typename tvalue, compator<tkey> compare>
red_black_tree<tkey, tvalue, compare>::postfix_reverse_iterator::postfix_reverse_iterator(parent::postfix_reverse_iterator it) noexcept
{
    throw not_implemented("template<typename tkey, typename tvalue, typename compare> red_black_tree<tkey, tvalue, compare>::postfix_reverse_iterator::postfix_reverse_iterator(parent::postfix_reverse_iterator it) noexcept", "your code should be here...");
}

template<typename tkey, typename tvalue, compator<tkey> compare>
typename red_black_tree<tkey, tvalue, compare>::node_color
red_black_tree<tkey, tvalue, compare>::postfix_reverse_iterator::get_color() const noexcept
{
    throw not_implemented("template<typename tkey, typename tvalue, typename compare> typename red_black_tree<tkey, tvalue, compare>::node_color red_black_tree<tkey, tvalue, compare>::postfix_reverse_iterator::get_color() const noexcept", "your code should be here...");
}

template<typename tkey, typename tvalue, compator<tkey> compare>
red_black_tree<tkey, tvalue, compare>::postfix_reverse_iterator::postfix_reverse_iterator(postfix_iterator it) noexcept
{
    throw not_implemented("template<typename tkey, typename tvalue, typename compare> red_black_tree<tkey, tvalue, compare>::postfix_reverse_iterator::postfix_reverse_iterator(postfix_iterator it) noexcept", "your code should be here...");
}

template<typename tkey, typename tvalue, compator<tkey> compare>
red_black_tree<tkey, tvalue, compare>::postfix_reverse_iterator::operator red_black_tree<tkey, tvalue, compare>::postfix_iterator() const noexcept
{
    throw not_implemented("template<typename tkey, typename tvalue, typename compare> red_black_tree<tkey, tvalue, compare>::postfix_reverse_iterator::operator postfix_iterator() const noexcept", "your code should be here...");
}

template<typename tkey, typename tvalue, compator<tkey> compare>
typename red_black_tree<tkey, tvalue, compare>::postfix_iterator
red_black_tree<tkey, tvalue, compare>::postfix_reverse_iterator::base() const noexcept
{
    throw not_implemented("template<typename tkey, typename tvalue, typename compare> typename red_black_tree<tkey, tvalue, compare>::postfix_iterator red_black_tree<tkey, tvalue, compare>::postfix_reverse_iterator::base() const noexcept", "your code should be here...");
}

template<typename tkey, typename tvalue, compator<tkey> compare>
red_black_tree<tkey, tvalue, compare>::postfix_const_reverse_iterator::postfix_const_reverse_iterator(parent::node* n) noexcept
{
    throw not_implemented("template<typename tkey, typename tvalue, typename compare> red_black_tree<tkey, tvalue, compare>::postfix_const_reverse_iterator::postfix_const_reverse_iterator(parent::node* n) noexcept", "your code should be here...");
}

template<typename tkey, typename tvalue, compator<tkey> compare>
red_black_tree<tkey, tvalue, compare>::postfix_const_reverse_iterator::postfix_const_reverse_iterator(parent::postfix_const_reverse_iterator it) noexcept
{
    throw not_implemented("template<typename tkey, typename tvalue, typename compare> red_black_tree<tkey, tvalue, compare>::postfix_const_reverse_iterator::postfix_const_reverse_iterator(parent::postfix_const_reverse_iterator it) noexcept", "your code should be here...");
}

template<typename tkey, typename tvalue, compator<tkey> compare>
typename red_black_tree<tkey, tvalue, compare>::node_color
red_black_tree<tkey, tvalue, compare>::postfix_const_reverse_iterator::get_color() const noexcept
{
    throw not_implemented("template<typename tkey, typename tvalue, typename compare> typename red_black_tree<tkey, tvalue, compare>::node_color red_black_tree<tkey, tvalue, compare>::postfix_const_reverse_iterator::get_color() const noexcept", "your code should be here...");
}

template<typename tkey, typename tvalue, compator<tkey> compare>
red_black_tree<tkey, tvalue, compare>::postfix_const_reverse_iterator::postfix_const_reverse_iterator(postfix_const_iterator it) noexcept
{
    throw not_implemented("template<typename tkey, typename tvalue, typename compare> red_black_tree<tkey, tvalue, compare>::postfix_const_reverse_iterator::postfix_const_reverse_iterator(postfix_const_iterator it) noexcept", "your code should be here...");
}

template<typename tkey, typename tvalue, compator<tkey> compare>
red_black_tree<tkey, tvalue, compare>::postfix_const_reverse_iterator::operator red_black_tree<tkey, tvalue, compare>::postfix_const_iterator() const noexcept
{
    throw not_implemented("template<typename tkey, typename tvalue, typename compare> red_black_tree<tkey, tvalue, compare>::postfix_const_reverse_iterator::operator postfix_const_iterator() const noexcept", "your code should be here...");
}

template<typename tkey, typename tvalue, compator<tkey> compare>
typename red_black_tree<tkey, tvalue, compare>::postfix_const_iterator
red_black_tree<tkey, tvalue, compare>::postfix_const_reverse_iterator::base() const noexcept
{
    throw not_implemented("template<typename tkey, typename tvalue, typename compare> typename red_black_tree<tkey, tvalue, compare>::postfix_const_iterator red_black_tree<tkey, tvalue, compare>::postfix_const_reverse_iterator::base() const noexcept", "your code should be here...");
}

// endregion iterator implementation

// region iterator requests implementation

template<typename tkey, typename tvalue, compator<tkey> compare>
typename red_black_tree<tkey, tvalue, compare>::infix_iterator
red_black_tree<tkey, tvalue, compare>::begin() noexcept
{
    throw not_implemented("template<typename tkey, typename tvalue, typename compare> typename red_black_tree<tkey, tvalue, compare>::infix_iterator red_black_tree<tkey, tvalue, compare>::begin() noexcept", "your code should be here...");
}

template<typename tkey, typename tvalue, compator<tkey> compare>
typename red_black_tree<tkey, tvalue, compare>::infix_iterator
red_black_tree<tkey, tvalue, compare>::end() noexcept
{
    throw not_implemented("template<typename tkey, typename tvalue, typename compare> typename red_black_tree<tkey, tvalue, compare>::infix_iterator red_black_tree<tkey, tvalue, compare>::end() noexcept", "your code should be here...");
}

template<typename tkey, typename tvalue, compator<tkey> compare>
typename red_black_tree<tkey, tvalue, compare>::infix_const_iterator
red_black_tree<tkey, tvalue, compare>::begin() const noexcept
{
    throw not_implemented("template<typename tkey, typename tvalue, typename compare> typename red_black_tree<tkey, tvalue, compare>::infix_const_iterator red_black_tree<tkey, tvalue, compare>::begin() const noexcept", "your code should be here...");
}

template<typename tkey, typename tvalue, compator<tkey> compare>
typename red_black_tree<tkey, tvalue, compare>::infix_const_iterator
red_black_tree<tkey, tvalue, compare>::end() const noexcept
{
    throw not_implemented("template<typename tkey, typename tvalue, typename compare> typename red_black_tree<tkey, tvalue, compare>::infix_const_iterator red_black_tree<tkey, tvalue, compare>::end() const noexcept", "your code should be here...");
}

template<typename tkey, typename tvalue, compator<tkey> compare>
typename red_black_tree<tkey, tvalue, compare>::infix_const_iterator
red_black_tree<tkey, tvalue, compare>::cbegin() const noexcept
{
    throw not_implemented("template<typename tkey, typename tvalue, typename compare> typename red_black_tree<tkey, tvalue, compare>::infix_const_iterator red_black_tree<tkey, tvalue, compare>::cbegin() const noexcept", "your code should be here...");
}

template<typename tkey, typename tvalue, compator<tkey> compare>
typename red_black_tree<tkey, tvalue, compare>::infix_const_iterator
red_black_tree<tkey, tvalue, compare>::cend() const noexcept
{
    throw not_implemented("template<typename tkey, typename tvalue, typename compare> typename red_black_tree<tkey, tvalue, compare>::infix_const_iterator red_black_tree<tkey, tvalue, compare>::cend() const noexcept", "your code should be here...");
}

template<typename tkey, typename tvalue, compator<tkey> compare>
typename red_black_tree<tkey, tvalue, compare>::infix_reverse_iterator
red_black_tree<tkey, tvalue, compare>::rbegin() noexcept
{
    throw not_implemented("template<typename tkey, typename tvalue, typename compare> typename red_black_tree<tkey, tvalue, compare>::infix_reverse_iterator red_black_tree<tkey, tvalue, compare>::rbegin() noexcept", "your code should be here...");
}

template<typename tkey, typename tvalue, compator<tkey> compare>
typename red_black_tree<tkey, tvalue, compare>::infix_reverse_iterator
red_black_tree<tkey, tvalue, compare>::rend() noexcept
{
    throw not_implemented("template<typename tkey, typename tvalue, typename compare> typename red_black_tree<tkey, tvalue, compare>::infix_reverse_iterator red_black_tree<tkey, tvalue, compare>::rend() noexcept", "your code should be here...");
}

template<typename tkey, typename tvalue, compator<tkey> compare>
typename red_black_tree<tkey, tvalue, compare>::infix_const_reverse_iterator
red_black_tree<tkey, tvalue, compare>::rbegin() const noexcept
{
    throw not_implemented("template<typename tkey, typename tvalue, typename compare> typename red_black_tree<tkey, tvalue, compare>::infix_const_reverse_iterator red_black_tree<tkey, tvalue, compare>::rbegin() const noexcept", "your code should be here...");
}

template<typename tkey, typename tvalue, compator<tkey> compare>
typename red_black_tree<tkey, tvalue, compare>::infix_const_reverse_iterator
red_black_tree<tkey, tvalue, compare>::rend() const noexcept
{
    throw not_implemented("template<typename tkey, typename tvalue, typename compare> typename red_black_tree<tkey, tvalue, compare>::infix_const_reverse_iterator red_black_tree<tkey, tvalue, compare>::rend() const noexcept", "your code should be here...");
}

template<typename tkey, typename tvalue, compator<tkey> compare>
typename red_black_tree<tkey, tvalue, compare>::infix_const_reverse_iterator
red_black_tree<tkey, tvalue, compare>::crbegin() const noexcept
{
    throw not_implemented("template<typename tkey, typename tvalue, typename compare> typename red_black_tree<tkey, tvalue, compare>::infix_const_reverse_iterator red_black_tree<tkey, tvalue, compare>::crbegin() const noexcept", "your code should be here...");
}

template<typename tkey, typename tvalue, compator<tkey> compare>
typename red_black_tree<tkey, tvalue, compare>::infix_const_reverse_iterator
red_black_tree<tkey, tvalue, compare>::crend() const noexcept
{
    throw not_implemented("template<typename tkey, typename tvalue, typename compare> typename red_black_tree<tkey, tvalue, compare>::infix_const_reverse_iterator red_black_tree<tkey, tvalue, compare>::crend() const noexcept", "your code should be here...");
}

template<typename tkey, typename tvalue, compator<tkey> compare>
typename red_black_tree<tkey, tvalue, compare>::prefix_iterator
red_black_tree<tkey, tvalue, compare>::begin_prefix() noexcept
{
    throw not_implemented("template<typename tkey, typename tvalue, typename compare> typename red_black_tree<tkey, tvalue, compare>::prefix_iterator red_black_tree<tkey, tvalue, compare>::begin_prefix() noexcept", "your code should be here...");
}

template<typename tkey, typename tvalue, compator<tkey> compare>
typename red_black_tree<tkey, tvalue, compare>::prefix_iterator
red_black_tree<tkey, tvalue, compare>::end_prefix() noexcept
{
    throw not_implemented("template<typename tkey, typename tvalue, typename compare> typename red_black_tree<tkey, tvalue, compare>::prefix_iterator red_black_tree<tkey, tvalue, compare>::end_prefix() noexcept", "your code should be here...");
}

template<typename tkey, typename tvalue, compator<tkey> compare>
typename red_black_tree<tkey, tvalue, compare>::prefix_const_iterator
red_black_tree<tkey, tvalue, compare>::begin_prefix() const noexcept
{
    throw not_implemented("template<typename tkey, typename tvalue, typename compare> typename red_black_tree<tkey, tvalue, compare>::prefix_const_iterator red_black_tree<tkey, tvalue, compare>::begin_prefix() const noexcept", "your code should be here...");
}

template<typename tkey, typename tvalue, compator<tkey> compare>
typename red_black_tree<tkey, tvalue, compare>::prefix_const_iterator
red_black_tree<tkey, tvalue, compare>::end_prefix() const noexcept
{
    throw not_implemented("template<typename tkey, typename tvalue, typename compare> typename red_black_tree<tkey, tvalue, compare>::prefix_const_iterator red_black_tree<tkey, tvalue, compare>::end_prefix() const noexcept", "your code should be here...");
}

template<typename tkey, typename tvalue, compator<tkey> compare>
typename red_black_tree<tkey, tvalue, compare>::prefix_const_iterator
red_black_tree<tkey, tvalue, compare>::cbegin_prefix() const noexcept
{
    throw not_implemented("template<typename tkey, typename tvalue, typename compare> typename red_black_tree<tkey, tvalue, compare>::prefix_const_iterator red_black_tree<tkey, tvalue, compare>::cbegin_prefix() const noexcept", "your code should be here...");
}

template<typename tkey, typename tvalue, compator<tkey> compare>
typename red_black_tree<tkey, tvalue, compare>::prefix_const_iterator
red_black_tree<tkey, tvalue, compare>::cend_prefix() const noexcept
{
    throw not_implemented("template<typename tkey, typename tvalue, typename compare> typename red_black_tree<tkey, tvalue, compare>::prefix_const_iterator red_black_tree<tkey, tvalue, compare>::cend_prefix() const noexcept", "your code should be here...");
}

template<typename tkey, typename tvalue, compator<tkey> compare>
typename red_black_tree<tkey, tvalue, compare>::prefix_reverse_iterator
red_black_tree<tkey, tvalue, compare>::rbegin_prefix() noexcept
{
    throw not_implemented("template<typename tkey, typename tvalue, typename compare> typename red_black_tree<tkey, tvalue, compare>::prefix_reverse_iteratorred_black_tree<tkey, tvalue, compare>::rbegin_prefix() noexcept", "your code should be here...");
}

template<typename tkey, typename tvalue, compator<tkey> compare>
typename red_black_tree<tkey, tvalue, compare>::prefix_reverse_iterator
red_black_tree<tkey, tvalue, compare>::rend_prefix() noexcept
{
    throw not_implemented("template<typename tkey, typename tvalue, typename compare> typename red_black_tree<tkey, tvalue, compare>::prefix_reverse_iterator red_black_tree<tkey, tvalue, compare>::rend_prefix() noexcept", "your code should be here...");
}

template<typename tkey, typename tvalue, compator<tkey> compare>
typename red_black_tree<tkey, tvalue, compare>::prefix_const_reverse_iterator
red_black_tree<tkey, tvalue, compare>::rbegin_prefix() const noexcept
{
    throw not_implemented("template<typename tkey, typename tvalue, typename compare> typename red_black_tree<tkey, tvalue, compare>::prefix_const_reverse_iterator red_black_tree<tkey, tvalue, compare>::rbegin_prefix() const noexcept", "your code should be here...");
}

template<typename tkey, typename tvalue, compator<tkey> compare>
typename red_black_tree<tkey, tvalue, compare>::prefix_const_reverse_iterator
red_black_tree<tkey, tvalue, compare>::rend_prefix() const noexcept
{
    throw not_implemented("template<typename tkey, typename tvalue, typename compare> typename red_black_tree<tkey, tvalue, compare>::prefix_const_reverse_iterator red_black_tree<tkey, tvalue, compare>::rend_prefix() const noexcept", "your code should be here...");
}

template<typename tkey, typename tvalue, compator<tkey> compare>
typename red_black_tree<tkey, tvalue, compare>::prefix_const_reverse_iterator
red_black_tree<tkey, tvalue, compare>::crbegin_prefix() const noexcept
{
    throw not_implemented("template<typename tkey, typename tvalue, typename compare> typename red_black_tree<tkey, tvalue, compare>::prefix_const_reverse_iterator red_black_tree<tkey, tvalue, compare>::crbegin_prefix() const noexcept", "your code should be here...");
}

template<typename tkey, typename tvalue, compator<tkey> compare>
typename red_black_tree<tkey, tvalue, compare>::prefix_const_reverse_iterator
red_black_tree<tkey, tvalue, compare>::crend_prefix() const noexcept
{
    throw not_implemented("template<typename tkey, typename tvalue, typename compare> typename red_black_tree<tkey, tvalue, compare>::prefix_const_reverse_iterator red_black_tree<tkey, tvalue, compare>::crend_prefix() const noexcept", "your code should be here...");
}

template<typename tkey, typename tvalue, compator<tkey> compare>
typename red_black_tree<tkey, tvalue, compare>::infix_iterator
red_black_tree<tkey, tvalue, compare>::begin_infix() noexcept
{
    throw not_implemented("template<typename tkey, typename tvalue, typename compare> typename red_black_tree<tkey, tvalue, compare>::infix_iterator red_black_tree<tkey, tvalue, compare>::begin_infix() noexcept", "your code should be here...");
}

template<typename tkey, typename tvalue, compator<tkey> compare>
typename red_black_tree<tkey, tvalue, compare>::infix_iterator
red_black_tree<tkey, tvalue, compare>::end_infix() noexcept
{
    throw not_implemented("template<typename tkey, typename tvalue, typename compare> typename red_black_tree<tkey, tvalue, compare>::infix_iterator red_black_tree<tkey, tvalue, compare>::end_infix() noexcept", "your code should be here...");
}

template<typename tkey, typename tvalue, compator<tkey> compare>
typename red_black_tree<tkey, tvalue, compare>::infix_const_iterator
red_black_tree<tkey, tvalue, compare>::begin_infix() const noexcept
{
    throw not_implemented("template<typename tkey, typename tvalue, typename compare> typename red_black_tree<tkey, tvalue, compare>::infix_const_iterator red_black_tree<tkey, tvalue, compare>::begin_infix() const noexcept", "your code should be here...");
}

template<typename tkey, typename tvalue, compator<tkey> compare>
typename red_black_tree<tkey, tvalue, compare>::infix_const_iterator
red_black_tree<tkey, tvalue, compare>::end_infix() const noexcept
{
    throw not_implemented("template<typename tkey, typename tvalue, typename compare> typename red_black_tree<tkey, tvalue, compare>::infix_const_iterator red_black_tree<tkey, tvalue, compare>::end_infix() const noexcept", "your code should be here...");
}

template<typename tkey, typename tvalue, compator<tkey> compare>
typename red_black_tree<tkey, tvalue, compare>::infix_const_iterator
red_black_tree<tkey, tvalue, compare>::cbegin_infix() const noexcept
{
    throw not_implemented("template<typename tkey, typename tvalue, typename compare> typename red_black_tree<tkey, tvalue, compare>::infix_const_iterator red_black_tree<tkey, tvalue, compare>::cbegin_infix() const noexcept", "your code should be here...");
}

template<typename tkey, typename tvalue, compator<tkey> compare>
typename red_black_tree<tkey, tvalue, compare>::infix_const_iterator
red_black_tree<tkey, tvalue, compare>::cend_infix() const noexcept
{
    throw not_implemented("template<typename tkey, typename tvalue, typename compare> typename red_black_tree<tkey, tvalue, compare>::infix_const_iterator red_black_tree<tkey, tvalue, compare>::cend_infix() const noexcept", "your code should be here...");
}

template<typename tkey, typename tvalue, compator<tkey> compare>
typename red_black_tree<tkey, tvalue, compare>::infix_reverse_iterator
red_black_tree<tkey, tvalue, compare>::rbegin_infix() noexcept
{
    throw not_implemented("template<typename tkey, typename tvalue, typename compare> typename red_black_tree<tkey, tvalue, compare>::infix_reverse_iterator red_black_tree<tkey, tvalue, compare>::rbegin_infix() noexcept", "your code should be here...");
}

template<typename tkey, typename tvalue, compator<tkey> compare>
typename red_black_tree<tkey, tvalue, compare>::infix_reverse_iterator
red_black_tree<tkey, tvalue, compare>::rend_infix() noexcept
{
    throw not_implemented("template<typename tkey, typename tvalue, typename compare> typename red_black_tree<tkey, tvalue, compare>::infix_reverse_iterator red_black_tree<tkey, tvalue, compare>::rend_infix() noexcept", "your code should be here...");
}

template<typename tkey, typename tvalue, compator<tkey> compare>
typename red_black_tree<tkey, tvalue, compare>::infix_const_reverse_iterator
red_black_tree<tkey, tvalue, compare>::rbegin_infix() const noexcept
{
    throw not_implemented("template<typename tkey, typename tvalue, typename compare> typename red_black_tree<tkey, tvalue, compare>::infix_const_reverse_iterator red_black_tree<tkey, tvalue, compare>::rbegin_infix() const noexcept", "your code should be here...");
}

template<typename tkey, typename tvalue, compator<tkey> compare>
typename red_black_tree<tkey, tvalue, compare>::infix_const_reverse_iterator
red_black_tree<tkey, tvalue, compare>::rend_infix() const noexcept
{
    throw not_implemented("template<typename tkey, typename tvalue, typename compare> typename red_black_tree<tkey, tvalue, compare>::infix_const_reverse_iterator red_black_tree<tkey, tvalue, compare>::rend_infix() const noexcept", "your code should be here...");
}

template<typename tkey, typename tvalue, compator<tkey> compare>
typename red_black_tree<tkey, tvalue, compare>::infix_const_reverse_iterator
red_black_tree<tkey, tvalue, compare>::crbegin_infix() const noexcept
{
    throw not_implemented("template<typename tkey, typename tvalue, typename compare> typename red_black_tree<tkey, tvalue, compare>::infix_const_reverse_iterator red_black_tree<tkey, tvalue, compare>::crbegin_infix() const noexcept", "your code should be here...");
}

template<typename tkey, typename tvalue, compator<tkey> compare>
typename red_black_tree<tkey, tvalue, compare>::infix_const_reverse_iterator
red_black_tree<tkey, tvalue, compare>::crend_infix() const noexcept
{
    throw not_implemented("template<typename tkey, typename tvalue, typename compare> typename red_black_tree<tkey, tvalue, compare>::infix_const_reverse_iterator red_black_tree<tkey, tvalue, compare>::crend_infix() const noexcept", "your code should be here...");
}

template<typename tkey, typename tvalue, compator<tkey> compare>
typename red_black_tree<tkey, tvalue, compare>::postfix_iterator
red_black_tree<tkey, tvalue, compare>::begin_postfix() noexcept
{
    throw not_implemented("template<typename tkey, typename tvalue, typename compare> typename red_black_tree<tkey, tvalue, compare>::postfix_iterator red_black_tree<tkey, tvalue, compare>::begin_postfix() noexcept", "your code should be here...");
}

template<typename tkey, typename tvalue, compator<tkey> compare>
typename red_black_tree<tkey, tvalue, compare>::postfix_iterator
red_black_tree<tkey, tvalue, compare>::end_postfix() noexcept
{
    throw not_implemented("template<typename tkey, typename tvalue, typename compare> typename red_black_tree<tkey, tvalue, compare>::postfix_iterator red_black_tree<tkey, tvalue, compare>::end_postfix() noexcept", "your code should be here...");
}

template<typename tkey, typename tvalue, compator<tkey> compare>
typename red_black_tree<tkey, tvalue, compare>::postfix_const_iterator
red_black_tree<tkey, tvalue, compare>::begin_postfix() const noexcept
{
    throw not_implemented("template<typename tkey, typename tvalue, typename compare> typename red_black_tree<tkey, tvalue, compare>::postfix_const_iterator red_black_tree<tkey, tvalue, compare>::begin_postfix() const noexcept", "your code should be here...");
}

template<typename tkey, typename tvalue, compator<tkey> compare>
typename red_black_tree<tkey, tvalue, compare>::postfix_const_iterator
red_black_tree<tkey, tvalue, compare>::end_postfix() const noexcept
{
    throw not_implemented("template<typename tkey, typename tvalue, typename compare> typename red_black_tree<tkey, tvalue, compare>::postfix_const_iterator red_black_tree<tkey, tvalue, compare>::end_postfix() const noexcept", "your code should be here...");
}

template<typename tkey, typename tvalue, compator<tkey> compare>
typename red_black_tree<tkey, tvalue, compare>::postfix_const_iterator
red_black_tree<tkey, tvalue, compare>::cbegin_postfix() const noexcept
{
    throw not_implemented("template<typename tkey, typename tvalue, typename compare> typename red_black_tree<tkey, tvalue, compare>::postfix_const_iterator red_black_tree<tkey, tvalue, compare>::cbegin_postfix() const noexcept", "your code should be here...");
}

template<typename tkey, typename tvalue, compator<tkey> compare>
typename red_black_tree<tkey, tvalue, compare>::postfix_const_iterator
red_black_tree<tkey, tvalue, compare>::cend_postfix() const noexcept
{
    throw not_implemented("template<typename tkey, typename tvalue, typename compare> typename red_black_tree<tkey, tvalue, compare>::postfix_const_iterator red_black_tree<tkey, tvalue, compare>::cend_postfix() const noexcept", "your code should be here...");
}

template<typename tkey, typename tvalue, compator<tkey> compare>
typename red_black_tree<tkey, tvalue, compare>::postfix_reverse_iterator
red_black_tree<tkey, tvalue, compare>::rbegin_postfix() noexcept
{
    throw not_implemented("template<typename tkey, typename tvalue, typename compare> typename red_black_tree<tkey, tvalue, compare>::postfix_reverse_iterator red_black_tree<tkey, tvalue, compare>::rbegin_postfix() noexcept", "your code should be here...");
}

template<typename tkey, typename tvalue, compator<tkey> compare>
typename red_black_tree<tkey, tvalue, compare>::postfix_reverse_iterator
red_black_tree<tkey, tvalue, compare>::rend_postfix() noexcept
{
    throw not_implemented("template<typename tkey, typename tvalue, typename compare> typename red_black_tree<tkey, tvalue, compare>::postfix_reverse_iterator red_black_tree<tkey, tvalue, compare>::rend_postfix() noexcept", "your code should be here...");
}

template<typename tkey, typename tvalue, compator<tkey> compare>
typename red_black_tree<tkey, tvalue, compare>::postfix_const_reverse_iterator
red_black_tree<tkey, tvalue, compare>::rbegin_postfix() const noexcept
{
    throw not_implemented("template<typename tkey, typename tvalue, typename compare> typename red_black_tree<tkey, tvalue, compare>::postfix_const_reverse_iterator red_black_tree<tkey, tvalue, compare>::rbegin_postfix() const noexcept", "your code should be here...");
}

template<typename tkey, typename tvalue, compator<tkey> compare>
typename red_black_tree<tkey, tvalue, compare>::postfix_const_reverse_iterator
red_black_tree<tkey, tvalue, compare>::rend_postfix() const noexcept
{
    throw not_implemented("template<typename tkey, typename tvalue, typename compare> typename red_black_tree<tkey, tvalue, compare>::postfix_const_reverse_iterator red_black_tree<tkey, tvalue, compare>::rend_postfix() const noexcept", "your code should be here...");
}

template<typename tkey, typename tvalue, compator<tkey> compare>
typename red_black_tree<tkey, tvalue, compare>::postfix_const_reverse_iterator
red_black_tree<tkey, tvalue, compare>::crbegin_postfix() const noexcept
{
    throw not_implemented("template<typename tkey, typename tvalue, typename compare> typename red_black_tree<tkey, tvalue, compare>::postfix_const_reverse_iterator red_black_tree<tkey, tvalue, compare>::crbegin_postfix() const noexcept", "your code should be here...");
}

template<typename tkey, typename tvalue, compator<tkey> compare>
typename red_black_tree<tkey, tvalue, compare>::postfix_const_reverse_iterator
red_black_tree<tkey, tvalue, compare>::crend_postfix() const noexcept
{
    throw not_implemented("template<typename tkey, typename tvalue, typename compare> typename red_black_tree<tkey, tvalue, compare>::postfix_const_reverse_iterator red_black_tree<tkey, tvalue, compare>::crend_postfix() const noexcept", "your code should be here...");
}

// endregion iterator requests implementation

// region rb_tree implementation

template<typename tkey, typename tvalue, compator<tkey> compare>
red_black_tree<tkey, tvalue, compare>::~red_black_tree() noexcept
{
    throw not_implemented("template<typename tkey, typename tvalue, typename compare> red_black_tree<tkey, tvalue, compare>::~red_black_tree() noexcept", "your code should be here...");
}

template<typename tkey, typename tvalue, compator<tkey> compare>
red_black_tree<tkey, tvalue, compare>::red_black_tree(red_black_tree const &other)
{
    throw not_implemented("template<typename tkey, typename tvalue, typename compare> red_black_tree<tkey, tvalue, compare>::red_black_tree(red_black_tree const &other)", "your code should be here...");
}

template<typename tkey, typename tvalue, compator<tkey> compare>
red_black_tree<tkey, tvalue, compare> &
red_black_tree<tkey, tvalue, compare>::operator=(red_black_tree const &other)
{
    throw not_implemented("template<typename tkey, typename tvalue, typename compare> red_black_tree<tkey, tvalue, compare> & red_black_tree<tkey, tvalue, compare>::operator=(red_black_tree const &other)", "your code should be here...");
}

template<typename tkey, typename tvalue, compator<tkey> compare>
red_black_tree<tkey, tvalue, compare>::red_black_tree(red_black_tree &&other) noexcept
{
    throw not_implemented("template<typename tkey, typename tvalue, typename compare> red_black_tree<tkey, tvalue, compare>::red_black_tree(red_black_tree &&other) noexcept", "your code should be here...");
}

template<typename tkey, typename tvalue, compator<tkey> compare>
red_black_tree<tkey, tvalue, compare> &
red_black_tree<tkey, tvalue, compare>::operator=(red_black_tree &&other) noexcept
{
    throw not_implemented("template<typename tkey, typename tvalue, typename compare> red_black_tree<tkey, tvalue, compare> & red_black_tree<tkey, tvalue, compare>::operator=(red_black_tree &&other) noexcept", "your code should be here...");
}

template<typename tkey, typename tvalue, compator<tkey> compare>
void red_black_tree<tkey, tvalue, compare>::swap(parent& other) noexcept
{
    throw not_implemented("template<typename tkey, typename tvalue, typename compare> void red_black_tree<tkey, tvalue, compare>::swap(parent& other) noexcept", "your code should be here...");
}

// endregion rb_tree implementation

template<typename tkey, typename tvalue, compator<tkey> compare>
std::pair<typename red_black_tree<tkey, tvalue, compare>::infix_iterator, bool>
red_black_tree<tkey, tvalue, compare>::insert(const value_type& value)
{
    throw not_implemented("template<typename tkey, typename tvalue, typename compare> std::pair<typename red_black_tree<tkey, tvalue, compare>::infix_iterator, bool> red_black_tree<tkey, tvalue, compare>::insert(const value_type& value)", "your code should be here...");
}

template<typename tkey, typename tvalue, compator<tkey> compare>
std::pair<typename red_black_tree<tkey, tvalue, compare>::infix_iterator, bool>
red_black_tree<tkey, tvalue, compare>::insert(value_type&& value)
{
    throw not_implemented("template<typename tkey, typename tvalue, typename compare> std::pair<typename red_black_tree<tkey, tvalue, compare>::infix_iterator, bool> red_black_tree<tkey, tvalue, compare>::insert(value_type&& value)", "your code should be here...");
}

template<typename tkey, typename tvalue, compator<tkey> compare>
template<class ...Args>
std::pair<typename red_black_tree<tkey, tvalue, compare>::infix_iterator, bool>
red_black_tree<tkey, tvalue, compare>::emplace(Args&&... args)
{
    throw not_implemented("template<typename tkey, typename tvalue, typename compare> template<class ...Args> std::pair<typename red_black_tree<tkey, tvalue, compare>::infix_iterator, bool> red_black_tree<tkey, tvalue, compare>::emplace(Args&&... args)", "your code should be here...");
}

template<typename tkey, typename tvalue, compator<tkey> compare>
typename red_black_tree<tkey, tvalue, compare>::infix_iterator
red_black_tree<tkey, tvalue, compare>::insert_or_assign(const value_type& value)
{
    throw not_implemented("template<typename tkey, typename tvalue, typename compare> typename red_black_tree<tkey, tvalue, compare>::infix_iterator red_black_tree<tkey, tvalue, compare>::insert_or_assign(const value_type& value)", "your code should be here...");
}

template<typename tkey, typename tvalue, compator<tkey> compare>
typename red_black_tree<tkey, tvalue, compare>::infix_iterator
red_black_tree<tkey, tvalue, compare>::insert_or_assign(value_type&& value)
{
    throw not_implemented("template<typename tkey, typename tvalue, typename compare> typename red_black_tree<tkey, tvalue, compare>::infix_iterator red_black_tree<tkey, tvalue, compare>::insert_or_assign(value_type&& value)", "your code should be here...");
}

template<typename tkey, typename tvalue, compator<tkey> compare>
template<class ...Args>
typename red_black_tree<tkey, tvalue, compare>::infix_iterator
red_black_tree<tkey, tvalue, compare>::emplace_or_assign(Args&&... args)
{
    throw not_implemented("template<typename tkey, typename tvalue, typename compare> template<class ...Args> typename red_black_tree<tkey, tvalue, compare>::infix_iterator red_black_tree<tkey, tvalue, compare>::emplace_or_assign(Args&&... args)", "your code should be here...");
}

template<typename tkey, typename tvalue, compator<tkey> compare>
typename red_black_tree<tkey, tvalue, compare>::infix_iterator
red_black_tree<tkey, tvalue, compare>::find(const tkey& key)
{
    throw not_implemented("template<typename tkey, typename tvalue, typename compare> typename red_black_tree<tkey, tvalue, compare>::infix_iterator red_black_tree<tkey, tvalue, compare>::find(const tkey& key)", "your code should be here...");
}

template<typename tkey, typename tvalue, compator<tkey> compare>
typename red_black_tree<tkey, tvalue, compare>::infix_const_iterator
red_black_tree<tkey, tvalue, compare>::find(const tkey& key) const
{
    throw not_implemented("template<typename tkey, typename tvalue, typename compare> typename red_black_tree<tkey, tvalue, compare>::infix_const_iterator red_black_tree<tkey, tvalue, compare>::find(const tkey& key) const", "your code should be here...");
}

template<typename tkey, typename tvalue, compator<tkey> compare>
typename red_black_tree<tkey, tvalue, compare>::infix_iterator
red_black_tree<tkey, tvalue, compare>::lower_bound(const tkey& key)
{
    throw not_implemented("template<typename tkey, typename tvalue, typename compare> typename red_black_tree<tkey, tvalue, compare>::infix_iterator red_black_tree<tkey, tvalue, compare>::lower_bound(const tkey& key)", "your code should be here...");
}

template<typename tkey, typename tvalue, compator<tkey> compare>
typename red_black_tree<tkey, tvalue, compare>::infix_const_iterator
red_black_tree<tkey, tvalue, compare>::lower_bound(const tkey& key) const
{
    throw not_implemented("template<typename tkey, typename tvalue, typename compare> typename red_black_tree<tkey, tvalue, compare>::infix_const_iterator red_black_tree<tkey, tvalue, compare>::lower_bound(const tkey& key) const", "your code should be here...");
}

template<typename tkey, typename tvalue, compator<tkey> compare>
typename red_black_tree<tkey, tvalue, compare>::infix_iterator
red_black_tree<tkey, tvalue, compare>::upper_bound(const tkey& key)
{
    throw not_implemented("template<typename tkey, typename tvalue, typename compare> typename red_black_tree<tkey, tvalue, compare>::infix_iterator red_black_tree<tkey, tvalue, compare>::upper_bound(const tkey& key)", "your code should be here...");
}

template<typename tkey, typename tvalue, compator<tkey> compare>
typename red_black_tree<tkey, tvalue, compare>::infix_const_iterator
red_black_tree<tkey, tvalue, compare>::upper_bound(const tkey& key) const
{
    throw not_implemented("template<typename tkey, typename tvalue, typename compare> typename red_black_tree<tkey, tvalue, compare>::infix_const_iterator red_black_tree<tkey, tvalue, compare>::upper_bound(const tkey& key) const", "your code should be here...");
}

template<typename tkey, typename tvalue, compator<tkey> compare>
typename red_black_tree<tkey, tvalue, compare>::infix_iterator
red_black_tree<tkey, tvalue, compare>::erase(infix_iterator pos)
{
    throw not_implemented("template<typename tkey, typename tvalue, typename compare> typename red_black_tree<tkey, tvalue, compare>::infix_iterator red_black_tree<tkey, tvalue, compare>::erase(infix_iterator pos)", "your code should be here...");
}

template<typename tkey, typename tvalue, compator<tkey> compare>
typename red_black_tree<tkey, tvalue, compare>::infix_iterator
red_black_tree<tkey, tvalue, compare>::erase(infix_const_iterator pos)
{
    throw not_implemented("template<typename tkey, typename tvalue, typename compare> typename red_black_tree<tkey, tvalue, compare>::infix_iterator red_black_tree<tkey, tvalue, compare>::erase(infix_const_iterator pos)", "your code should be here...");
}

template<typename tkey, typename tvalue, compator<tkey> compare>
typename red_black_tree<tkey, tvalue, compare>::infix_iterator
red_black_tree<tkey, tvalue, compare>::erase(infix_iterator first, infix_iterator last)
{
    throw not_implemented("template<typename tkey, typename tvalue, typename compare> typename red_black_tree<tkey, tvalue, compare>::infix_iterator red_black_tree<tkey, tvalue, compare>::erase(infix_iterator first, infix_iterator last)", "your code should be here...");
}

template<typename tkey, typename tvalue, compator<tkey> compare>
typename red_black_tree<tkey, tvalue, compare>::infix_iterator
red_black_tree<tkey, tvalue, compare>::erase(infix_const_iterator first, infix_const_iterator last)
{
    throw not_implemented("template<typename tkey, typename tvalue, typename compare> typename red_black_tree<tkey, tvalue, compare>::infix_iterator red_black_tree<tkey, tvalue, compare>::erase(infix_const_iterator first, infix_const_iterator last)", "your code should be here...");
}

#endif //MATH_PRACTICE_AND_OPERATING_SYSTEMS_RED_BLACK_TREE_H
