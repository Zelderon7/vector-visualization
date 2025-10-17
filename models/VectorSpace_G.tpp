#pragma once

template<typename T>
T VectorSpace<T>::get_plane_normal(const T *v1, const T *v2) requires is_vector3_v<T> {
    return (*v1 * *v2).normalized();
}

template<typename T>
std::pair<T, T> VectorSpace<T>::get_plane_basis(const T *v1, const T *n) requires is_vector3_v<T> {
    T u = *v1;
    u.normalized();

    T v = n * u;
    v.normalized();

    return {u, v};
}