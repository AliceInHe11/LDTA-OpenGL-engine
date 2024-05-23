#ifndef AUDIO_H
#define AUDIO_H

#include <fmod_studio.hpp>
#include <fmod.hpp>
#include <fmod_errors.h>
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <map>
#include "soundinfo.h"


// * Error Handling Function for FMOD Errors
// * @param result - the FMOD_RESULT generated during every FMOD 


void ERRCHECK_fn(FMOD_RESULT result, const char* file, int line)
{
    if (result != FMOD_OK)
        std::cout << "FMOD ERROR: Audio [Line " << line << "] " << result << "  - " << FMOD_ErrorString(result) << '\n';
}

#define ERRCHECK(_result) ERRCHECK_fn(_result, __FILE__, __LINE__)
// Error checking/debugging function definitions


// * Class that handles the process of loading and playing sounds by wrapping FMOD's functionality.
// * Deals with all FMOD calls so that FMOD-specific code does not need to be used outside this class.
// * Only one AudioEngine should be constructed for an application.

class AudioEngine {
public:
    // The audio sampling rate of the audio engine
    static const int AUDIO_SAMPLE_RATE = 44100;
    
    //  Default AudioEngine constructor. 
    //  AudioEngine::init() must be called before using the Audio Engine 
    
    AudioEngine() : sounds(), loopsPlaying(), soundBanks(),
    eventDescriptions(), eventInstances() {}

    
    // Initializes Audio Engine Studio and Core systems to default values. 
    
    void init()
    {
        ERRCHECK(FMOD::Studio::System::create(&studioSystem));
        ERRCHECK(studioSystem->getCoreSystem(&lowLevelSystem));
        ERRCHECK(lowLevelSystem->setSoftwareFormat(AUDIO_SAMPLE_RATE, FMOD_SPEAKERMODE_STEREO, 0));
        ERRCHECK(lowLevelSystem->set3DSettings(1.0, DISTANCEFACTOR, 0.5f));
        ERRCHECK(studioSystem->initialize(MAX_AUDIO_CHANNELS, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, 0));
        ERRCHECK(lowLevelSystem->getMasterChannelGroup(&mastergroup));
        initReverb();
    }

    /**
     * Method that is called to deactivate the audio engine after use.
     */
    void deactivate()
    {
        lowLevelSystem->close();
        studioSystem->release();
    }

    /**
    * Method which should be called every frame of the game loop
    */
    void update()
    {
        ERRCHECK(studioSystem->update()); // also updates the low level system
    }
    
    /**
     * Loads a sound from disk using provided settings
     * Prepares for later playback with playSound()
     * Only reads the audio file and loads into the audio engine
     * if the sound file has already been added to the cache
     */
    void loadSound(SoundInfo soundInfo)
    {
        if (!soundInfo.isLoaded()) {
            SET_COLOR(LIGHTGREEN);
            std::cout << "Audio Engine: Loading Sound from file " << soundInfo.getFilePath() << '\n';
            SET_COLOR(WHITE);
            FMOD::Sound* sound;
            ERRCHECK(lowLevelSystem->createSound(soundInfo.getFilePath(), soundInfo.is3D() ? FMOD_3D : FMOD_2D, 0, &sound));
            ERRCHECK(sound->setMode(soundInfo.isLoop() ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF));
            ERRCHECK(sound->set3DMinMaxDistance(0.5f * DISTANCEFACTOR, 5000.0f * DISTANCEFACTOR));
            sounds.insert({ soundInfo.getUniqueID(), sound });
            unsigned int msLength = 0;
            ERRCHECK(sounds[soundInfo.getUniqueID()]->getLength(&msLength, FMOD_TIMEUNIT_MS));
            //soundInfo.setMSLength(msLength);
            soundInfo.setLoaded(SOUND_LOADED);
        }
        else 
        {
            SET_COLOR(RED);
            std::cout << "Audio Engine: Sound File was already loaded!\n";
            SET_COLOR(WHITE);
        }
    }

    /**
    * Plays a sound file using FMOD's low level audio system. If the sound file has not been
    * previously loaded using loadSoundFile(), a console message is displayed
    *
    * @param filename - relative path to file from project directory. (Can be .OGG, .WAV, .MP3,
    *                 or any other FMOD-supported audio format)
    */
    void playSound(SoundInfo soundInfo)
    {
        if (!soundInfo.isLoaded()) {
            //std::cout << "Playing Sound\n";
            FMOD::Channel* channel;
            // start play in 'paused' state
            ERRCHECK(lowLevelSystem->playSound(sounds[soundInfo.getUniqueID()], 0, true /* start paused */, &channel));

            if (soundInfo.is3D())
                set3dChannelPosition(soundInfo, channel);

            //std::cout << "Playing sound at volume " << soundInfo.getVolume() << '\n';
            channel->setVolume(soundInfo.getVolume());

            if (soundInfo.isLoop()) // add to channel map of sounds currently playing, to stop later
                loopsPlaying.insert({ soundInfo.getUniqueID(), channel });

            ERRCHECK(channel->setReverbProperties(0, soundInfo.getReverbAmount()));

            // start audio playback
            ERRCHECK(channel->setPaused(false));

        }
        else 
        {
            SET_COLOR(RED);
            std::cout << "Audio Engine: Can't play, sound was not loaded yet from " << soundInfo.getFilePath() << '\n';
            SET_COLOR(WHITE);
        }
    }
    
    /**
     * Stops a looping sound if it's currently playing.
     */
    void stopSound(SoundInfo soundInfo)
    {
        if (soundIsPlaying(soundInfo)) {
            ERRCHECK(loopsPlaying[soundInfo.getUniqueID()]->stop());
            loopsPlaying.erase(soundInfo.getUniqueID());
        }
        else
        {
            SET_COLOR(RED);
            std::cout << "Audio Engine: Can't stop a looping sound that's not playing!\n";
            SET_COLOR(WHITE);
        }
    }

    /**
     * Method that updates the volume of a soundloop that is playing. This can be used to create audio 'fades'
     * where the volume ramps up or down to the provided new volume
     * @param fadeSampleLength the length in samples of the intended volume sample. If less than 64 samples, the default 
     *                         FMOD fade out is used
     */
    void updateSoundLoopVolume(SoundInfo& soundInfo, float newVolume, unsigned int fadeSampleLength = 0)
    {
        if (soundIsPlaying(soundInfo)) {
            FMOD::Channel* channel = loopsPlaying[soundInfo.getUniqueID()];
            if (fadeSampleLength <= 64) // 64 samples is default volume fade out
                ERRCHECK(channel->setVolume(newVolume));
            else {
                bool fadeUp = newVolume > soundInfo.getVolume();
                // get current audio clock time
                unsigned long long parentclock = 0;
                ERRCHECK(channel->getDSPClock(NULL, &parentclock));

                float targetFadeVol = fadeUp ? 1.0f : newVolume;

                if (fadeUp) ERRCHECK(channel->setVolume(newVolume));

                ERRCHECK(channel->addFadePoint(parentclock, soundInfo.getVolume()));
                ERRCHECK(channel->addFadePoint(parentclock + fadeSampleLength, targetFadeVol));
                //std::cout << "Current DSP Clock: " << parentclock << ", fade length in samples  = " << fadeSampleLength << "\n";
            }
            //std::cout << "Updating with new soundinfo vol \n";
            soundInfo.setVolume(newVolume); // update the SoundInfo's volume
        }
        else
        {
            SET_COLOR(RED);
            std::cout << "AudioEngine: Can't update sound loop volume! (It isn't playing or might not be loaded)\n";
            SET_COLOR(WHITE);
        }
    }

   

    /**
    * Updates the position of a looping 3D sound that has already been loaded and is playing back.
    * The SoundInfo object's position coordinates will be used for the new sound position, so
    * SoundInfo::set3DCoords(x,y,z) should be called before this method to set the new desired location.
    */
    void update3DSoundPosition(SoundInfo soundInfo)
    {
        if (soundIsPlaying(soundInfo))
            set3dChannelPosition(soundInfo, loopsPlaying[soundInfo.getUniqueID()]);
        else
        {
            SET_COLOR(RED);
            std::cout << "Audio Engine: Can't update sound position!\n";
            SET_COLOR(WHITE);
        }

    }
      
    /**
     * Checks if a looping sound is playing.
     */
    bool soundIsPlaying(SoundInfo soundInfo)
    {
        return soundInfo.isLoop() && loopsPlaying.count(soundInfo.getUniqueID());
    }
   

    /**
     * Sets the position of the listener in the 3D scene.
     * @param posX, posY, posZ - 3D translation of listener
     * @param forwardX, forwardY, forwardZ - forward angle character is looking in
     * @param upX, upY, upZ - up which must be perpendicular to forward vector
     */
    void set3DListenerPosition(float posX, float posY, float posZ,
                               float forwardX, float forwardY, float forwardZ,
                               float upX, float upY, float upZ)
    {
        listenerpos = { posX,     posY,     posZ };
        forward = { forwardX, forwardY, forwardZ };
        up = { upX,      upY,      upZ };
        ERRCHECK(lowLevelSystem->set3DListenerAttributes(0, &listenerpos, 0, &forward, &up));
    }

    /**
    * Utility method that returns the length of a SoundInfo's audio file in milliseconds
    * If the sound hasn't been loaded, returns 0
    */
    unsigned int getSoundLengthInMS(SoundInfo soundInfo)
    {
        unsigned int length = 0;
        if (sounds.count(soundInfo.getUniqueID()))
            ERRCHECK(sounds[soundInfo.getUniqueID()]->getLength(&length, FMOD_TIMEUNIT_MS));
        return length;
    }

    /**
     * Loads an FMOD Studio soundbank 
     * TODO Fix
     */
    void loadFMODStudioBank(const char* filePath)
    {
        std::cout << "Audio Engine: Loading FMOD Studio Sound Bank " << filePath << '\n';
        FMOD::Studio::Bank* bank = NULL;
        ERRCHECK(studioSystem->loadBankFile(filePath, FMOD_STUDIO_LOAD_BANK_NORMAL, &bank));
        soundBanks.insert({ filePath, bank });
    }
    
    /**
     * Loads an FMOD Studio Event. The Soundbank that this event is in must have been loaded before
     * calling this method.
     * TODO Fix
     */
    void loadFMODStudioEvent(const char* eventName, std::vector<std::pair<const char*, float>> paramsValues = { })
    {
        std::cout << "AudioEngine: Loading FMOD Studio Event " << eventName << '\n';
        FMOD::Studio::EventDescription* eventDescription = NULL;
        ERRCHECK(studioSystem->getEvent(eventName, &eventDescription));
        // Create an instance of the event
        FMOD::Studio::EventInstance* eventInstance = NULL;
        ERRCHECK(eventDescription->createInstance(&eventInstance));
        for (const auto& parVal : paramsValues) {
            std::cout << "AudioEngine: Setting Event Instance Parameter " << parVal.first << "to value: " << parVal.second << '\n';
            // Set the parameter values of the event instance
            ERRCHECK(eventInstance->setParameterByName(parVal.first, parVal.second));
        }
        eventInstances.insert({ eventName, eventInstance });
        eventDescriptions.insert({ eventName, eventDescription });
    }
    
    /**
     * Sets the parameter of an FMOD Soundbank Event Instance.
     */
    void setFMODEventParamValue(const char* eventName, const char* parameterName, float value)
    {
        if (eventInstances.count(eventName) > 0)
            ERRCHECK(eventInstances[eventName]->setParameterByName(parameterName, value));
        else
            std::cout << "AudioEngine: Event " << eventName << " was not in event instance cache, can't set param \n";
    }
    
    /**
     * Plays the specified instance of an event
     * TODO support playback of multiple event instances
     * TODO Fix playback
     */
    void playEvent(const char* eventName, int instanceIndex = 0)
    {
        // printEventInfo(eventDescriptions[eventName]);
        auto eventInstance = eventInstances[eventName];
        if (eventInstances.count(eventName) > 0)
            ERRCHECK(eventInstances[eventName]->start());
        else
            std::cout << "AudioEngine: Event " << eventName << " was not in event instance cache, cannot play \n";
    }
    
    /**
     * Stops the specified instance of an event, if it is playing.
     */
    void stopEvent(const char* eventName, int instanceIndex = 0)
    {
        if (eventInstances.count(eventName) > 0)
            ERRCHECK(eventInstances[eventName]->stop(FMOD_STUDIO_STOP_ALLOWFADEOUT));
        else
            std::cout << "AudioEngine: Event " << eventName << " was not in event instance cache, cannot stop \n";
    }
 
    /**
     * Sets the volume of an event.
     * @param volume0to1 - volume of the event, from 0 (min vol) to 1 (max vol)
     */
    void setEventVolume(const char* eventName, float volume0to1 = .75f)
    {
        std::cout << "AudioEngine: Setting Event Volume\n";
        ERRCHECK(eventInstances[eventName]->setVolume(volume0to1));
    }

    /**
     * Checks if an event is playing.
     */
    bool eventIsPlaying(const char* eventName, int instance = 0)
    {
        FMOD_STUDIO_PLAYBACK_STATE playbackState;
        ERRCHECK(eventInstances[eventName]->getPlaybackState(&playbackState));
        return playbackState == FMOD_STUDIO_PLAYBACK_PLAYING;
    }

    /**
     * Mutes all sounds for the audio engine
     */
    void muteAllSounds()
    {
        ERRCHECK(mastergroup->setMute(true));
        muted = true;
    }

    /**
     * Unmutes all sounds for the audio engine
     */
    void unmuteAllSound()
    {
        ERRCHECK(mastergroup->setMute(false));
        muted = false;
    }

    /**
     * Returns true if the audio engine is muted, false if not
     */
    bool isMuted()
    {
        return muted;
    }

private:  

    /**
     * Checks if a sound file is in the soundCache
     */
    bool soundLoaded(SoundInfo soundInfo)
    {
        //std::cout << "Checking sound " << soundInfo.getUniqueID() << " exists\n";
        return sounds.count(soundInfo.getUniqueID()) > 0;
    }

    /**
     * Sets the 3D position of a sound
     */
    void set3dChannelPosition(SoundInfo soundInfo, FMOD::Channel* channel)
    {
        FMOD_VECTOR position = { soundInfo.getX() * DISTANCEFACTOR, soundInfo.getY() * DISTANCEFACTOR, soundInfo.getZ() * DISTANCEFACTOR };
        FMOD_VECTOR velocity = { 0.0f, 0.0f, 0.0f }; // TODO Add dopplar (velocity) support
        ERRCHECK(channel->set3DAttributes(&position, &velocity));
    }

    /**
     * Initializes the reverb effect
     */
    void initReverb()
    {
        ERRCHECK(lowLevelSystem->createReverb3D(&reverb));
        FMOD_REVERB_PROPERTIES prop2 = FMOD_PRESET_CONCERTHALL;
        ERRCHECK(reverb->setProperties(&prop2));
        ERRCHECK(reverb->set3DAttributes(&revPos, revMinDist, revMaxDist));
    }

    /**
     * Prints debug info about an FMOD event description
     */
    void printEventInfo(FMOD::Studio::EventDescription* eventDescription)
    {
        int params;
        bool is3D, isOneshot;
        ERRCHECK(eventDescription->getParameterDescriptionCount(&params));
        ERRCHECK(eventDescription->is3D(&is3D));
        ERRCHECK(eventDescription->isOneshot(&isOneshot));

        std::cout << "FMOD EventDescription has " << params << " parameter descriptions, "
            << (is3D ? " is " : " isn't ") << " 3D, "
            << (isOneshot ? " is " : " isn't ") << " oneshot, "
            << (eventDescription->isValid() ? " is " : " isn't ") << " valid."
            << '\n';
    }

    // FMOD Studio API system, which can play FMOD sound banks (*.bank)
    FMOD::Studio::System* studioSystem = nullptr;       
    
    // FMOD's low-level audio system which plays audio files and is obtained from Studio System
    FMOD::System* lowLevelSystem = nullptr;          

    // Max FMOD::Channels for the audio engine 
    static const unsigned int MAX_AUDIO_CHANNELS = 1024; 
    
    // Units per meter.  I.e feet would = 3.28.  centimeters would = 100.
    const float DISTANCEFACTOR = 1.0f;  
 
    // Listener head position, initialized to default value
    FMOD_VECTOR listenerpos = { 0.0f, 0.0f, -1.0f * DISTANCEFACTOR };
    
    // Listener forward vector, initialized to default value
    FMOD_VECTOR forward     = { 0.0f, 0.0f, 1.0f };
    
    // Listener upwards vector, initialized to default value
    FMOD_VECTOR up          = { 0.0f, 1.0f, 0.0f };

    // Main group for low level system which all sounds go though
    FMOD::ChannelGroup* mastergroup = 0;

    // Low-level system reverb TODO add multi-reverb support
	FMOD::Reverb3D* reverb;

	// Reverb origin position
	FMOD_VECTOR revPos = { 0.0f, 0.0f, 0.0f };

	// reverb min, max distances
	float revMinDist = 10.0f, revMaxDist = 50.0f;

    // flag tracking if the Audio Engin is muted
    bool muted = false;

    /*
     * Map which caches FMOD Low-Level sounds
     * Key is the SoundInfo's uniqueKey field.
     * Value is the FMOD::Sound* to be played back.
     * TODO Refactor to use numeric UID as key
     */
    std::map<std::string, FMOD::Sound*> sounds;

    /*
     * Map which stores the current playback channels of any playing sound loop
     * Key is the SoundInfo's uniqueKey field.
     * Value is the FMOD::Channel* the FMOD::Sound* is playing back on.
     */
    std::map<std::string, FMOD::Channel*> loopsPlaying;

    /*
     * Map which stores the soundbanks loaded with loadFMODStudioBank()
     */
    std::map<std::string, FMOD::Studio::Bank*> soundBanks;
    
    /*
     * Map which stores event descriptions created during loadFMODStudioEvent()
     */
    std::map<std::string, FMOD::Studio::EventDescription*> eventDescriptions;
    
    /*
     * Map which stores event instances created during loadFMODStudioEvent()
     */
    std::map<std::string, FMOD::Studio::EventInstance*> eventInstances;
};

#endif
