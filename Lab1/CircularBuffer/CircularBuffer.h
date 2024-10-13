typedef char value_type;

class CircularBuffer
{
private:
    value_type *buffer;

    int head;
    int tail;

    int capacity;
    int size;

public:
    // Constructors and destructor
    CircularBuffer();
    ~CircularBuffer();
    CircularBuffer(const CircularBuffer &cb); // (!)
    explicit CircularBuffer(int capacity);
    CircularBuffer(int capacity, const value_type &elem);


    // Operators
    value_type &operator[](int i);
    const value_type &operator[](int i) const;
    value_type &at(int i);
    const value_type &at(int i) const;
    CircularBuffer &operator=(const CircularBuffer &cb);


    // Geters
    int GetHead() const;
    int GetTail() const;
    int GetCapacity() const;
    int GetSize() const;
    int GetReserve() const;

    value_type &Front();
    value_type &Back();
    const value_type &Front() const;
    const value_type &Back() const;


    //Seters
    void SetHead(int ind);
    void SetTail(int ind);
    void SetCapacity(int new_capacity);
    void SetSize(int new_size);


    // Checkers
    bool Full() const;
    bool Empty() const;
    bool IsLinearized() const;


    // Push and pop
    void PushBack(const value_type &item = value_type());
    void PushFront(const value_type &item = value_type());
    void PopBack();
    void PopFront();


    // Other
    value_type *Linearize();
    void Rotate(int new_begin);
    void Resize(int new_size, const value_type &item = value_type());
    void Swap(CircularBuffer &cb);
    void Insert(int pos, const value_type &item = value_type());
    void Erase(int first, int last);
    void Clear();
};

bool operator==(const CircularBuffer &first, const CircularBuffer &second);
bool operator!=(const CircularBuffer &first, const CircularBuffer &second);