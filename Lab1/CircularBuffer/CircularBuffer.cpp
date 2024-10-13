#include "CircularBuffer.h"
#include <iostream>

// Constructors and destructor
CircularBuffer::CircularBuffer() {}

CircularBuffer::~CircularBuffer()
{
    delete[] buffer;
    std::cout << "Deleted: " << this << std::endl;
}

CircularBuffer::CircularBuffer(const CircularBuffer &cb)
{
    buffer = new value_type[cb.capacity]{};
    for (int i = 0; i < cb.capacity; i++) {
        buffer[i] = cb.buffer[i];
    }
    head = cb.head;
    tail = cb.tail;
    capacity = cb.capacity;
    size = cb.size;

    std::cout << "Created_with_cb: " << this << std::endl;
}

CircularBuffer::CircularBuffer(int capacity)
{
    buffer = new value_type[capacity]{};
    head = 0;
    tail = 0;
    this->capacity = capacity;
    size = 0;
    
    std::cout << "Created_with_capacity: " << this << std::endl;
}

CircularBuffer::CircularBuffer(int capacity, const value_type &elem) : CircularBuffer(capacity)
{
    for (int i = 0; i < capacity; i++) {
        buffer[i] = elem;
    }
    size = capacity;

    std::cout << "Filling: " << this << std::endl;
}



// Operators
value_type &CircularBuffer::operator[](int i) {
    return buffer[i];
}

const value_type &CircularBuffer::operator[](int i) const {
    return buffer[i];
}

value_type &CircularBuffer::at(int i)
{
    if (i >= capacity || i < -1 * capacity)
        throw std::out_of_range("Index out of range!\n");
    else
        return buffer[(i + capacity) % capacity];
}

const value_type &CircularBuffer::at(int i) const
{    
    if (i >= capacity || i < -1 * capacity)
        throw std::out_of_range("Index out of range!\n");
    else
        return buffer[(i + capacity) % capacity];
}

CircularBuffer &CircularBuffer::operator=(const CircularBuffer &cb)
{
    if (this == &cb)
        return *this;

    delete[] buffer;
    value_type *new_buffer = new value_type[cb.capacity]{};

    for (int i = cb.head; i < cb.head + cb.size; i++) {
        new_buffer[i % cb.capacity] = cb.buffer[i % cb.capacity];
    }
    this->buffer = new_buffer;

    head = cb.head;
    tail = cb.tail;
    capacity = cb.capacity;
    size = cb.size;

    return *this;
}



// Geters
int CircularBuffer::GetHead() const {
    return head;
}

int CircularBuffer::GetTail() const {
    return tail;
}

int CircularBuffer::GetCapacity() const {
    return capacity;
}

int CircularBuffer::GetSize() const {
    return size;
}

int CircularBuffer::GetReserve() const {
    return capacity - size;
}

value_type &CircularBuffer::Front() {
    return buffer[head];
}

value_type &CircularBuffer::Back() {
    return buffer[(tail - 1 + capacity) % capacity];
}

const value_type &CircularBuffer::Front() const {
    return buffer[head];
}

const value_type &CircularBuffer::Back() const {
    return buffer[(tail - 1 + capacity) % capacity];
}



//Seters
void CircularBuffer::SetHead(int ind) {
    head = ind;
}

void CircularBuffer::SetTail(int ind) {
    tail = ind;
}

void CircularBuffer::SetCapacity(int new_capacity)
{
    if (new_capacity != capacity)
    {
        value_type *new_buffer = new value_type[new_capacity]{};

        if (new_capacity < capacity)
        {
            for (int i = head; i < head + new_capacity; i++) {
                new_buffer[i % new_capacity] = buffer[i % capacity];
            }
            tail = head;
            
            capacity = new_capacity;
            size = new_capacity;
        }
        else
        {
            for (int i = head; i < head + size; i++) {
                new_buffer[i % new_capacity] = buffer[i % capacity];
            }
            tail = (head + size) % new_capacity;
            capacity = new_capacity;
        }
        delete[] buffer;
        this->buffer = new_buffer;
    }
}

void CircularBuffer::SetSize(int new_size) {
    size = new_size;
}



// Checkers
bool CircularBuffer::Full() const
{
    if (size == capacity)
        return true;
    return false;
}

bool CircularBuffer::Empty() const
{
    if (size == 0)
        return true;
    return false;
}

bool CircularBuffer::IsLinearized() const
{
    if (head == 0)
        return true;
    return false;
}

// Push and pop
void CircularBuffer::PushBack(const value_type &item)
{
    buffer[tail] = item;
    tail = (tail + 1) % capacity;
    
    if (!Full())
        size++;
}

void CircularBuffer::PushFront(const value_type &item)
{
    head = (head - 1 + capacity) % capacity;
    buffer[head] = item;
    
    if (!Full())
        size++;
}

void CircularBuffer::PopBack()
{
    if (Empty())
        throw std::underflow_error("Buffer is empty, cannot remove an item.\n");

    tail = (tail - 1 + capacity) % capacity;
    buffer[tail] = value_type();
}

void CircularBuffer::PopFront()
{
    if (Empty())
        throw std::underflow_error("Buffer is empty, cannot remove an item.\n");
    
    buffer[head] = value_type();
    head = (head + 1) % capacity;
}



// Other
value_type *CircularBuffer::Linearize()
{
    value_type *new_buffer = new value_type[capacity]{};

    for (int i = 0; i < size; i++) {
        new_buffer[i] = buffer[(head + i) % capacity];
    }
    delete[] buffer;
    this->buffer = new_buffer;

    head = 0;
    tail = size % capacity;

    return this->buffer;
}

void CircularBuffer::Rotate(int new_begin)
{
    value_type *new_buffer = new value_type[capacity]{};

    for (int i = 0; i < capacity - new_begin; i++) {
        new_buffer[i] = buffer[new_begin + i];
    }
    for (int i = 1; i <= new_begin; i++) {
        new_buffer[capacity - i] = buffer[new_begin - i];
    }
    delete[] buffer;
    this->buffer = new_buffer;

    head = (head - new_begin + capacity) % capacity;
    tail = (tail - new_begin + capacity) % capacity;
}

void CircularBuffer::Resize(int new_size, const value_type &item)
{
    if (new_size != size)
    {
        if (new_size < size)
        {
            for (int i = 0; i < size - new_size; i++) {
                buffer[(head + new_size + i) % capacity] = value_type();
            }
            tail = (tail - (size - new_size) + capacity) % capacity;
            size = new_size;
        }
        else
        {
            if (new_size <= capacity)
            {
                for (int i = 0; i < new_size - size; i++) {
                    PushBack(item);
                }
            }
            else
            {
                SetCapacity(new_size);
                for (int i = 0; i < new_size - size; i++) {
                    buffer[(tail + i) % capacity] = item;
                }
                tail = (tail + (new_size - size)) % capacity;
                size += new_size - size;
            }
        }
    }
}

void CircularBuffer::Swap(CircularBuffer &cb)
{
    if (capacity != cb.capacity)
        throw std::invalid_argument("The capacities are not the same.\n");
    else
    {
        {
            value_type temp;
            for (int i = 0; i < capacity; i++)
            {
                temp = cb.buffer[i];
                cb.buffer[i] = buffer[i];
                buffer[i] = temp;
            }
        }
        int temp;

        temp = cb.GetHead();
        cb.SetHead(head);
        head = temp;

        temp = cb.GetTail();
        cb.SetTail(tail);
        tail = temp;

        temp = cb.GetSize();
        cb.SetSize(size);
        size = temp;
    }
}

void CircularBuffer::Insert(int pos, const value_type &item) {
    buffer[pos] = item;
}

void CircularBuffer::Erase(int first, int last)
{
    if (first < 0 || last > capacity || first > last)
        throw std::invalid_argument("Incorrect boundaries.");

    for (int i = first; i < last; i++) {
        buffer[i] = value_type();
    }
}

void CircularBuffer::Clear()
{
    for (int i = 0; i < capacity; i++) {
        PopBack();
    }
}

bool operator==(const CircularBuffer &first, const CircularBuffer &second)
{
    if (first.GetHead() == second.GetHead() &&
        first.GetTail() == second.GetTail() &&
        first.GetCapacity() == second.GetCapacity() &&
        first.GetSize() == second.GetSize())
    {
        for (int i = 0; i < first.GetCapacity(); i++) {
            if (first[i] != second[i])
                return false;
        }
        return true;
    }
    return false;
}

bool operator!=(const CircularBuffer &first, const CircularBuffer &second)
{
    if (first.GetHead() == second.GetHead() &&
        first.GetTail() == second.GetTail() &&
        first.GetCapacity() == second.GetCapacity() &&
        first.GetSize() == second.GetSize())
    {
        for (int i = 0; i < first.GetCapacity(); i++) {
            if (first[i] != second[i])
                return true;
        }
        return false;
    }
    return true;
}