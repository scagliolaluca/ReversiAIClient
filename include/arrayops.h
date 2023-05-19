#ifndef ARRAYOPS_H
#define ARRAYOPS_H

#include <cstdlib>
#include <cstring>

template <class T>
T** create2DArr(size_t sizeFirst, size_t sizeSecond) {
    T** newArr = new T*[sizeFirst];

    for (size_t i = 0; i < sizeFirst; i++) {
        newArr[i] = new T[sizeSecond];
    }

    return newArr;
}

template <class T>
T** createZeroInit2DArr(size_t sizeFirst, size_t sizeSecond) {
    T** newArr = new T*[sizeFirst];

    for (size_t i = 0; i < sizeFirst; i++) {
        newArr[i] = new T[sizeSecond]();
    }

    return newArr;
}

template <class T>
void delete2DArr(T** arr, size_t sizeFirst) {
    if (!arr) {
        return;
    }

    for (size_t i = 0; i < sizeFirst; i++) {
        delete[] arr[i];
    }

    delete[] arr;
    arr = nullptr;
}

template <class T>
T** copy2DArr(T** arr, size_t sizeFirst, size_t sizeSecond) {
    T** newArr = create2DArr<T>(sizeFirst, sizeSecond);

    size_t byteCount = sizeof(T) * sizeSecond;

    for (size_t i = 0; i < sizeFirst; i++) {
        memcpy(newArr[i], arr[i], byteCount);
    }

    return newArr;
}

template <class T>
T* copy1DArr(T* arr, size_t size) {
    T* newArr = new T[size];

    memcpy(newArr, arr, sizeof(T) * size);

    return newArr;
}

#endif  // ARRAYOPS_H