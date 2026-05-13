//Calculates distance between points on sphere of radius R
//-90 <= lat <= 90, 0 <= long <= 360
template<typename T>
T distance_between_points_on_sphere(T lat1, T long1, T lat2, T long2, T R) {
    static_assert(is_floating_point_v<T>);
    assert(-90 <= min(lat1, lat2) && max(lat1, lat2) <= 90);
    assert(0 <= min(long1, long2) && max(long1, long2) <= 360);
    static constexpr T PI = 3.14159265358979323846264;
    lat1 *= PI / 180, long1 *= PI / 180;
    lat2 *= PI / 180, long2 *= PI / 180;
    T cl1 = cosl(lat1), cl2 = cosl(lat2);
    T sl1 = sinl(lat1), sl2 = sinl(lat2);
    T delta = long2 - long1;
    T cdelta = cosl(delta), sdelta = sinl(delta);
    T y = sqrtl(powl(cl2 * sdelta, 2) + powl(cl1 * sl2 - sl1 * cl2 * cdelta, 2));
    T x = sl1 * sl2 + cl1 * cl2 * cdelta;
    T ad = atan2l(y, x);
    return ad * R;
}
