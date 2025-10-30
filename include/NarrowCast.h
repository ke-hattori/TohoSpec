#ifndef NARROWCAST_H
#define NARROWCAST_H

// Helper functions for safe narrowing conversions
// Used to eliminate C4244/C4267 warnings while maintaining explicit intent

// Simple narrowing cast - makes intent explicit without runtime checks
template<typename T, typename U>
inline T narrow_cast(U v) {
    return static_cast<T>(v);
}

// Clamping cast - clamps value to target type range before casting
// Use at API boundaries where range is well-defined
template<typename T, typename U>
inline T clamp_cast(U v, T lo, T hi) {
    return (v < lo) ? lo : (v > hi) ? hi : static_cast<T>(v);
}

#endif // NARROWCAST_H
