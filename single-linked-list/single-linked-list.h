#pragma once

#include <cassert>
#include <cstddef>
#include <string>
#include <utility>
#include <initializer_list>

template <typename Type>
class SingleLinkedList {
	
    // Узел списка
    struct Node {
        Node() = default;
        Node(const Type& val, Node* next)
            : value(val)
            , next_node(next) {
        }

        Type value;
        Node* next_node = nullptr;
    };
	
    template <typename ValueType>
    class BasicIterator
    {
        template <typename T>
        friend class SingleLinkedList;
        explicit BasicIterator(Node* node)
        {
            pointer_ = node;
        }
        
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = Type;
        using difference_type = std::ptrdiff_t;
        using pointer = ValueType*;
        using reference = ValueType&;

        BasicIterator() = default;

        BasicIterator(const BasicIterator<Type>& other)
            : BasicIterator(other.pointer_)
        {
        }

        BasicIterator& operator=(const BasicIterator& rhs) = default;

        [[nodiscard]] bool operator==(const BasicIterator<const Type>& rhs) const noexcept
        {
            return pointer_ == rhs.pointer_;
        }
        [[nodiscard]] bool operator==(BasicIterator<Type>& rhs) const noexcept
        {
            return pointer_ == rhs.pointer_;
        }
        [[nodiscard]] bool operator!=(const BasicIterator<const Type>& rhs) const
        {
            return pointer_ != rhs.pointer_;
        }
        [[nodiscard]] bool operator!=(BasicIterator<Type>& rhs) const noexcept
        {
            return pointer_ != rhs.pointer_;
        }

        BasicIterator& operator++() noexcept
        {
            pointer_ = pointer_->next_node;
            return *this;
        }

        BasicIterator operator++(int) noexcept
        {
            BasicIterator it = Iterator(pointer_);
            ++(*this);
            return it;
        }

        [[nodiscard]] reference operator*() const noexcept
        {
            return pointer_->value;
        }

        [[nodiscard]] pointer operator->() const noexcept
        {
            return &(pointer_->value);
        }

    private:
        Node* pointer_ = nullptr;
    };
	
public:
    using value_type = Type;
    using reference = value_type&;
    using const_reference = const value_type&;

    using Iterator = BasicIterator<Type>;
    using ConstIterator = BasicIterator<const Type>;

    [[nodiscard]] Iterator begin() noexcept
    {
        return Iterator(head_.next_node);
    }

    [[nodiscard]] Iterator end() noexcept
    {
        Node* node = head_.next_node;
        while (node)
        {
            node = node->next_node;

        }
        return Iterator(node);
    }

    [[nodiscard]] ConstIterator begin() const noexcept
    {
        return ConstIterator(head_.next_node);
    }

    [[nodiscard]] ConstIterator end() const noexcept
    {
        Node* node = head_.next_node;
        while (node)
        {
            node = node->next_node;

        }
        return ConstIterator(node);
    }

    [[nodiscard]] ConstIterator cbegin() const noexcept
    {
        return ConstIterator(head_.next_node);
    }

    [[nodiscard]] ConstIterator cend() const noexcept
    {
        Node* node = head_.next_node;
        while (node)
        {
            node = node->next_node;
        }
        return ConstIterator(node);
    }
    
    [[nodiscard]] Iterator before_begin() noexcept
    {
        return Iterator(head_p);
    }
    
    [[nodiscard]] ConstIterator before_begin() const noexcept
    {
        return ConstIterator(head_p);
    }
    
    [[nodiscard]] ConstIterator cbefore_begin() const noexcept
    {
        return ConstIterator(head_p);
    }

    SingleLinkedList() 
	{
		
	};

    void PushFront(Type value)
    {
        head_.next_node = new Node(value, head_.next_node);
        ++size_;
    }
    
    void PopFront() noexcept
    {
        Node* tmp = head_.next_node;
        head_.next_node = head_.next_node->next_node;
        --size_;
        delete tmp;
    }

    Iterator InsertAfter(ConstIterator it, const Type& value)
    {
        Node* inserted_node = new Node(value, it.pointer_->next_node);
        it.pointer_->next_node = inserted_node;
        ++size_;
        ++it;
        return Iterator(it.pointer_);
    }

    Iterator EraseAfter(ConstIterator it) noexcept
    {
        Node* node_to_delete = it.pointer_->next_node;
        it.pointer_->next_node = it.pointer_->next_node->next_node;
        delete node_to_delete;
        --size_;
        return Iterator(it.pointer_->next_node);
    }

    SingleLinkedList(std::initializer_list<Type> il)
    {
        for (auto it = il.end() - 1; it >= il.begin(); --it)
        {
            PushFront(*it);
        }
    }

    SingleLinkedList(const SingleLinkedList<Type>& rhs)
    {
        assert(size_ == 0 && head_.next_node == nullptr);
        SingleLinkedList tmp1, tmp2;
        for (ConstIterator it = rhs.cbegin(); it != rhs.cend(); ++it)
        {
            tmp1.PushFront(*it);
        }
        for (auto it = tmp1.begin(); it != tmp1.end(); ++it)
        {
            tmp2.PushFront(*it);
        }
        swap(tmp2);
    }

    bool operator==(const SingleLinkedList& other)
    {
        return std::equal(this->begin(), this->end(), other.begin(), other.end());
    }

    bool operator!=(const SingleLinkedList& other)
    {
        return !(*this == other);
    }

    bool operator<(const SingleLinkedList& other)
    {
        return std::lexicographical_compare(this->begin(), this->end(), other.begin(), other.end());
    }

    bool operator>=(const SingleLinkedList& other)
    {
        return !(*this < other);
    }

    bool operator<=(const SingleLinkedList& other)
    {
        return (*this < other) || (*this == other);
    }

    bool operator>(const SingleLinkedList& other)
    {
        return !(*this <= other);
    }

    SingleLinkedList& operator=(const SingleLinkedList& rhs)
    {
        SingleLinkedList tmp(rhs);
        swap(tmp);
        return *this;
    }

    void Clear()
    {
        while (head_.next_node)
        {
            Node* tmp = head_.next_node->next_node;
            delete head_.next_node;
            head_.next_node = tmp;
        }
        size_ = 0;
    }

    void swap(SingleLinkedList& rhs) noexcept
    {
        Node* tmp = rhs.head_.next_node;
        rhs.head_.next_node = this->head_.next_node;
        this->head_.next_node = tmp;
        this->size_ = this->size_ ^ rhs.size_;
        rhs.size_ = this->size_ ^ rhs.size_;
        this->size_ = this->size_ ^ rhs.size_;
    }

    ~SingleLinkedList()
    {
        Clear();
    }

    // Возвращает количество элементов в списке за время O(1)
    [[nodiscard]] size_t GetSize() const noexcept {
        return size_;
    }

    // Сообщает, пустой ли список за время O(1)
    [[nodiscard]] bool IsEmpty() const noexcept {
        return !size_;
    }

private:
    // Фиктивный узел, используется для вставки "перед первым элементом"
    Node head_;
    Node* head_p = &head_;
    size_t size_ = 0;
};

template <typename Type>
void swap(SingleLinkedList<Type>& lhs, SingleLinkedList<Type>& rhs) noexcept
{
    lhs.swap(rhs);
}
