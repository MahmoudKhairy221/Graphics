#ifndef AUDIO_H
#define AUDIO_H

#include <string>

class Audio {
public:
    static void Initialize();
    static void Shutdown();
    
    // Sound effect functions (placeholder)
    static void PlayPickupSound();
    static void PlayCollisionSound();
    static void PlayConsoleSound();
    static void PlayPortalSound();
    static void PlayAlarmSound();
    
    // Load sound from file (placeholder)
    static unsigned int LoadSound(const std::string& path);
    static void PlaySound(unsigned int soundID);
    
private:
    static bool m_initialized;
};

#endif // AUDIO_H

