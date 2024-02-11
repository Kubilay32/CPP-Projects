#ifndef HASH_TABLE_HPP
#define HASH_TABLE_HPP

//=======================//
// Implemented Functions //
//=======================//
template<int MAX_SIZE>
int HashTable<MAX_SIZE>::PRIMES[3] = {102523, 100907, 104659};

template<int MAX_SIZE>
void HashTable<MAX_SIZE>::PrintLine(int tableIndex) const
{
    const HashData& data = table[tableIndex];

    // Using printf here it is easier to format
    if(data.sentinel == SENTINEL_MARK)
    {
        printf("[%03d]         : SENTINEL\n", tableIndex);
    }
    else if(data.sentinel == EMPTY_MARK)
    {
        printf("[%03d]         : EMPTY\n", tableIndex);
    }
    else
    {
        printf("[%03d] - [%03d] : ", tableIndex, data.lruCounter);
        printf("(%-5s) ", data.isCostWeighted ? "True" : "False");
        size_t sz = data.intArray.size();
        for(size_t i = 0; i < sz; i++)
        {
            if(i % 2 == 0)
                printf("[%03d]", data.intArray[i]);
            else
                printf("/%03d/", data.intArray[i]);

            if(i != sz - 1)
                printf("-->");
        }
        printf("\n");
    }
}

template<int MAX_SIZE>
void HashTable<MAX_SIZE>::PrintTable() const
{
    printf("____________________\n");
    printf("Elements %d\n", elementCount);
    printf("[IDX] - [LRU] | DATA\n");
    printf("____________________\n");
    for(int i = 0; i < MAX_SIZE; i++)
    {
        PrintLine(i);
    }
}

//=======================//
//          TODO         //
//=======================//
template<int MAX_SIZE>
int HashTable<MAX_SIZE>::Hash(int startInt, int endInt, bool isCostWeighted)
{
    /* TODO */
    int flag;
    if(isCostWeighted == false){
        flag = 0;
    }
    else{
        flag = 1;
    }
    
    int hash;
    
    hash = PRIMES[0] * startInt + PRIMES[1] * endInt + PRIMES[2] * flag;
    
    return hash % MAX_SIZE;
}

template<int MAX_SIZE>
HashTable<MAX_SIZE>::HashTable()
{
    /* TODO */
    for(int i = 0; i < MAX_SIZE; i++){
        table[i].sentinel = EMPTY_MARK;
        table[i].lruCounter = 0;
    }
    elementCount = 0;
}

template<int MAX_SIZE>
int HashTable<MAX_SIZE>::Insert(const std::vector<int>& intArray, bool isCostWeighted)
{
    /* TODO */
    if(intArray.empty()){
        throw InvalidTableArgException();
    }
    
    int key = Hash(intArray.front(), intArray.back(), isCostWeighted);
    
    int tempKey = key;
    int j = 1;
    
    
    
    while(table[key].sentinel != EMPTY_MARK && table[key].sentinel != SENTINEL_MARK){
        if(table[key].intArray[0] == intArray[0] 
            && table[key].intArray[table[key].intArray.size() - 1] == intArray[intArray.size() - 1] 
            && table[key].isCostWeighted == isCostWeighted){
                    
            table[key].lruCounter++;
            table[key].sentinel = OCCUPIED_MARK;
            table[key].isCostWeighted = isCostWeighted;
            return table[key].lruCounter;
        }
        key = (tempKey + j * j) % MAX_SIZE;
        j++;
    }
    
    
    
    int temp = tempKey;
    int i = 1;
    
    if(elementCount > MAX_SIZE / 2){
        throw TableCapFullException(elementCount);
    }
    
    while(table[tempKey].sentinel != EMPTY_MARK && table[tempKey].sentinel != SENTINEL_MARK){
        tempKey = (temp + i * i) % MAX_SIZE;
        i++;         
    }

    for(int i = 0; i < intArray.size(); i++){
        table[tempKey].intArray.push_back(intArray[i]);
    }
    
    if(table[key].lruCounter == 0){
        table[key].lruCounter = 1;
    }
    
    table[tempKey].startInt = table[tempKey].intArray.front();
    table[tempKey].endInt = table[tempKey].intArray.back();
    table[tempKey].sentinel = OCCUPIED_MARK;
    table[tempKey].isCostWeighted = isCostWeighted;
    elementCount++;

    return 0;
    
}

template<int MAX_SIZE>
bool HashTable<MAX_SIZE>::Find(std::vector<int>& intArray,
                               int startInt, int endInt, bool isCostWeighted,
                               bool incLRU)
{
    /* TODO */
    int isTrue = 0;
    int j = 1;
    int hashValue = Hash(startInt, endInt, isCostWeighted);
    int temp = hashValue;
    
    while(table[hashValue].sentinel != EMPTY_MARK && table[hashValue].sentinel != SENTINEL_MARK){
        if(table[hashValue].intArray.front() == startInt
            && table[hashValue].intArray.back() == endInt
            && table[hashValue].isCostWeighted == isCostWeighted){
                isTrue = 1;
                break;
        }
    
        hashValue = (temp + j * j) % MAX_SIZE;
        j++;
    }
    
    
    
    if(isTrue == 1){
        if(incLRU == true){
            table[hashValue].lruCounter++;
        }
        intArray.clear();
        for(int i = 0; i < table[hashValue].intArray.size(); i++){
            intArray.push_back(table[hashValue].intArray[i]);
        }
        return true;
    }
    
    else if(isTrue == 0){
        return false;
    }
}

template<int MAX_SIZE>
void HashTable<MAX_SIZE>::InvalidateTable()
{
    /* TODO */
    for(int i = 0; i < MAX_SIZE; i++){
        if(table[i].sentinel != EMPTY_MARK && table[i].sentinel != SENTINEL_MARK){
            while(!table[i].intArray.empty()){
                table[i].intArray.pop_back();
            }
            table[i].sentinel = EMPTY_MARK;
            elementCount--;
            table[i].lruCounter = 0;
        }
        if(table[i].sentinel == SENTINEL_MARK){
            table[i].sentinel = EMPTY_MARK;
        }
    }
}

template<int MAX_SIZE>
void HashTable<MAX_SIZE>::GetMostInserted(std::vector<int>& intArray) const
{
    /* TODO */
    int maxIndex = -1;
    int maxLruCounter = 0;
    
    for(int i = 0; i < MAX_SIZE; i++){
        if(table[i].sentinel != EMPTY_MARK && table[i].sentinel != SENTINEL_MARK){
            if(table[i].lruCounter > maxLruCounter){
                maxIndex = i;
                maxLruCounter = table[i].lruCounter;
            }        
        }
    }
    
    if(maxIndex == -1){
        return;
    }
    
    else{
        while(!intArray.empty()){
            intArray.pop_back();
        }
        
        for(int j = 0; j < table[maxIndex].intArray.size(); j++){
            intArray.push_back(table[maxIndex].intArray[j]);
        }
    }
}

template<int MAX_SIZE>
void HashTable<MAX_SIZE>::Remove(std::vector<int>& intArray,
                                 int startInt, int endInt, bool isCostWeighted)
{
    /* TODO */
    int index = -1;
    int j = 1;
    int hashValue = Hash(startInt, endInt, isCostWeighted);
    int temp = hashValue;
    
    while(table[hashValue].sentinel != EMPTY_MARK && table[hashValue].sentinel != SENTINEL_MARK){
        if(table[hashValue].intArray.front() == startInt
            && table[hashValue].intArray.back() == endInt
            && table[hashValue].isCostWeighted == isCostWeighted){
                index = hashValue;
                break;
        }
    
        hashValue = (temp + j * j) % MAX_SIZE;
        j++;
    }
    
    if(index == -1){
        return;
    }
    
    while(!intArray.empty()){
        intArray.pop_back();
    }
        
    for(int i = 0; i < table[index].intArray.size(); i++){
        intArray.push_back(table[index].intArray[i]);
    }
    
    
    while(!table[index].intArray.empty()){
        table[index].intArray.pop_back();
    }
    
    table[index].sentinel = SENTINEL_MARK;
    table[index].lruCounter = 0; 
    elementCount--;
    
}

template<int MAX_SIZE>
void HashTable<MAX_SIZE>::RemoveLRU(int lruElementCount)
{
    /* TODO */
    int lowestLru = 100000000;

    for(int i = 0; i < MAX_SIZE; i++){
        if(table[i].lruCounter < lowestLru){
            lowestLru = table[i].lruCounter;
        }  
    }
    
    while(lruElementCount != 0){
        for(int i = 0; i < MAX_SIZE; i++){
            if(table[i].sentinel == OCCUPIED_MARK){
                if(table[i].lruCounter == lowestLru){
                    while(!table[i].intArray.empty()){
                        table[i].intArray.pop_back();
                    }
                    table[i].sentinel = SENTINEL_MARK;
                    table[i].lruCounter = 0;
                    elementCount--;
                    lruElementCount--;
                }
            }
            
            if(lruElementCount == 0){
                break;
            }
        }
    
        lowestLru++;
    }
}
template<int MAX_SIZE>
void HashTable<MAX_SIZE>::PrintSortedLRUEntries() const
{
    /* TODO */
    int highestLru = 0;
    
    for(int i = 0; i < MAX_SIZE; i++){
        if(table[i].lruCounter > highestLru){
            highestLru = table[i].lruCounter;
        }  
    }
    
    int temp = elementCount;
    
    
    while(temp != 0){
        for(int i = 0; i < MAX_SIZE; i++){
            if(table[i].sentinel == OCCUPIED_MARK){
                if(table[i].lruCounter == highestLru){
                    PrintLine(i);
                    temp--;
                }  
            } 
            
            if(temp == 0){
                break;
            }
        }
        highestLru--;
        
    }
    
}

#endif // HASH_TABLE_HPP
