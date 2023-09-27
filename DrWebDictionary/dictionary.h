#pragma once
#include <memory>

template<class Key>
class not_found_exception : public std::exception
{
public:
    virtual const Key& get_key() const noexcept = 0;
};

template<class Key, class Value>
class dictionary
{
public:
    using not_found_exception_type = not_found_exception<Key>;

    virtual ~dictionary() = default;

    virtual const Value& get(const Key& key) const = 0;
    virtual void set(const Key& key, const Value& value) = 0;
    virtual bool is_set(const Key& key) const = 0; 

};


template<class Key>
class not_found_in_map_exception : public not_found_exception<Key>
{
public:
    virtual const Key& get_key() const noexcept
    {
        return key;
    }

    not_found_in_map_exception(const Key& key) {
        this->key = key;
    }

private:
    Key key;
};

template<class Key, class Value>
class map : public dictionary<Key, Value>
{
public:
    virtual const Value& get(const Key& key) const override;
    virtual void set(const Key& key, const Value& value) override;
    virtual bool is_set(const Key& key) const override;

    ~map()
    {
        delete data;
    }

private:
    //class for key value pairs
    struct Pair
    {
        Key key;
        Value value;

        Pair() = default;
        Pair(const Key& key, const Value& value) 
        {
            this->key = key;
            this->value = value;
        }
    };

    //array container for the key-value pairs 
    //needs to be updated once its capacity is reached
    const size_t startCapacity = 10;
    const float capacityMultiplier = 1.5f;
    Pair* data = new Pair[startCapacity];
    size_t capacity = startCapacity;
    size_t currentSize = 0;

    void UpdateCapacity() 
    {
        //create a new array of bigger size
        size_t newCapacity = size_t(capacity * capacityMultiplier);
        auto newData = new Pair[newCapacity];

        // copy data to the new array
        std::memcpy(newData, data, sizeof(Pair) * currentSize);

        //delete old array
        delete data;

        // update object values
        capacity = newCapacity;
        data = newData;
    }

};

template<class Key, class Value>
const Value& map<Key, Value>::get(const Key& key) const
{
    // iterate through the array to find a corresponding element
    for (size_t i = 0; i < currentSize; ++i) 
    {
        if (data[i].key == key) 
        {
            return data[i].value;
        }
    }

    // if the value is not found throw a not_found_exception
    throw not_found_in_map_exception<Key>(key);
}

template<class Key, class Value>
void map<Key, Value>::set(const Key& key, const Value& value)
{
    //check if key aready exists in the collection
    Pair* pair = nullptr;
    for (int i = 0; i < currentSize; ++i) 
    {
        if (data[i].key == key) 
        {
            pair = &data[i];
            break;
        }
    }

    // if key is found, update value
    if (pair)
    {
        pair->value = value;
    }
    // if key is not found add a new key-value pair
    else
    {
        //if capacity is reached update capcity
        if (currentSize == capacity) 
        {
            UpdateCapacity();
        }
        data[currentSize] = Pair(key, value);
        ++currentSize;
    }
}

template<class Key, class Value>
bool map<Key, Value>::is_set(const Key& key) const
{

    for (int i = 0; i < currentSize; ++i) 
    {
        if (data[i].key == key) 
        {
            return true;
        }
    }
    return false;
}
