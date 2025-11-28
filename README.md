# Playter

| ID | Name |
|:---|:----------------------|
| K24-2510 | Ahad |
| K24-2561 | Aayan Hassan |

## Introduction
Playter is a lightweight MP3 player simulation built using **C++** with the **Qt framework** for its graphical interface. The project focuses on managing and playing songs efficiently through the use of fundamental data structures. It aims to handle song addition, removal, searching, and playlist organization while supporting basic playback controls like play, pause, next, and previous. The emphasis will remain on the internal data handling and logic rather than complex audio processing.

## Description
- Users can add, remove, and search for songs.  
- Playlists can be created, deleted, and modified.  
- Playback includes play, pause, next, and previous functions.  
- GUI built with **Qt** to simulate player controls and display playlists.  
- Queue-based mechanism for playback order.  
- Stack-based system for recently played songs.  
- Hashing or indexing for efficient song lookups.  
- Future improvements may include shuffle, repeat, and metadata-based sorting.  

## Data Structures that will be used

| Data Structure | Usage | Description |
|:----------------|:--------|:-------------|
| **Linked List** | Song and playlist storage | Each song will be a node, allowing dynamic addition, removal, and traversal. |
| **Queue** | Playback queue | Maintains the order of upcoming songs. |
| **Stack** | Playback history | Stores recently played songs for the “previous” functionality. |
| **Hash Map** | Fast lookup | Enables efficient search of songs by title or artist. |
| **Array / Vector** | Playlist collection | Holds multiple playlists for iteration and management. |
