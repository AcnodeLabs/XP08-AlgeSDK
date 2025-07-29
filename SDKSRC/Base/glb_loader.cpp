#ifndef  GLB_LOADER.CPP

#define GLB_LOADER_CPP


#include <stdint.h>
// Implementation functions
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


namespace glb_loader {

    // GLB file header (12 bytes)
    typedef struct {
        uint32_t magic;     // Must be 0x46546C67 ("glTF" in ASCII)
        uint32_t version;   // Version of the GLB format (usually 2)
        uint32_t length;    // Total length of the GLB file in bytes
    } GLBHeader;

    // GLB chunk header (8 bytes)
    typedef struct {
        uint32_t chunkLength;  // Length of the chunk data in bytes
        uint32_t chunkType;    // Type of chunk (JSON or BIN)
    } GLBChunkHeader;

    // Chunk types
#define GLB_CHUNK_TYPE_JSON 0x4E4F534A  // "JSON" in ASCII
#define GLB_CHUNK_TYPE_BIN  0x004E4942  // "BIN\0" in ASCII

// Complete GLB structure for loading
    typedef struct {
        GLBHeader header;

        // JSON chunk
        GLBChunkHeader jsonChunkHeader;
        char* jsonData;  // Pointer to JSON data (null-terminated)

        // Binary chunk (optional)
        GLBChunkHeader binChunkHeader;
        uint8_t* binData;  // Pointer to binary data

        // For memory management
        uint8_t* fileData;  // Original file data buffer
        size_t fileSize;    // Size of the file
    } GLBFile;

    // Error codes for GLB loading
    typedef enum {
        GLB_SUCCESS = 0,
        GLB_ERROR_FILE_TOO_SMALL,
        GLB_ERROR_INVALID_MAGIC,
        GLB_ERROR_UNSUPPORTED_VERSION,
        GLB_ERROR_INVALID_LENGTH,
        GLB_ERROR_INVALID_JSON_CHUNK,
        GLB_ERROR_MEMORY_ALLOCATION,
        GLB_ERROR_INVALID_FILE
    } GLBResult;

    // Function prototypes for GLB loading
    GLBResult loadGLB(const char* filename, GLBFile* glbFile);
    GLBResult parseGLBFromMemory(const uint8_t* data, size_t size, GLBFile* glbFile);
    void freeGLB(GLBFile* glbFile);

    // Helper function to validate GLB magic number
    static inline int isValidGLBMagic(uint32_t magic) {
        return magic == 0x46546C67;  // "glTF"
    }

    // Example usage structure for parsed data
    typedef struct {
        GLBFile glb;

        // Parsed JSON data would go here
        // This would typically be parsed using a JSON library like cJSON
        void* gltfScene;  // Pointer to your parsed glTF scene structure

        // Direct access to binary buffers
        struct {
            uint8_t* vertices;
            uint8_t* indices;
            uint8_t* textures;
            // ... other buffer views
        } buffers;
    } GLTFModel;



    GLBResult loadGLB(const char* filename, GLBFile* glbFile) {
        FILE* file = fopen(filename, "rb");
        if (!file) {
            return GLB_ERROR_INVALID_FILE;
        }

        // Get file size
        fseek(file, 0, SEEK_END);
        size_t fileSize = ftell(file);
        fseek(file, 0, SEEK_SET);

        if (fileSize < sizeof(GLBHeader)) {
            fclose(file);
            return GLB_ERROR_FILE_TOO_SMALL;
        }

        // Allocate buffer for entire file
        uint8_t* fileData = (uint8_t*) malloc(fileSize);
        if (!fileData) {
            fclose(file);
            return GLB_ERROR_MEMORY_ALLOCATION;
        }

        // Read entire file
        if (fread(fileData, 1, fileSize, file) != fileSize) {
            free(fileData);
            fclose(file);
            return GLB_ERROR_INVALID_FILE;
        }
        fclose(file);

        GLBResult result = parseGLBFromMemory(fileData, fileSize, glbFile);
        if (result != GLB_SUCCESS) {
            free(fileData);
            return result;
        }

        glbFile->fileData = fileData;
        glbFile->fileSize = fileSize;
        return GLB_SUCCESS;
    }

    GLBResult parseGLBFromMemory(const uint8_t* data, size_t size, GLBFile* glbFile) {
        if (size < sizeof(GLBHeader)) {
            return GLB_ERROR_FILE_TOO_SMALL;
        }

        // Parse header
        memcpy(&glbFile->header, data, sizeof(GLBHeader));

        // Validate magic number
        if (!isValidGLBMagic(glbFile->header.magic)) {
            return GLB_ERROR_INVALID_MAGIC;
        }

        // Check version (usually 2)
        if (glbFile->header.version != 2) {
            return GLB_ERROR_UNSUPPORTED_VERSION;
        }

        // Validate file length
        if (glbFile->header.length != size) {
            return GLB_ERROR_INVALID_LENGTH;
        }

        size_t offset = sizeof(GLBHeader);

        // Parse JSON chunk
        if (offset + sizeof(GLBChunkHeader) > size) {
            return GLB_ERROR_INVALID_JSON_CHUNK;
        }

        memcpy(&glbFile->jsonChunkHeader, data + offset, sizeof(GLBChunkHeader));
        offset += sizeof(GLBChunkHeader);

        if (glbFile->jsonChunkHeader.chunkType != GLB_CHUNK_TYPE_JSON) {
            return GLB_ERROR_INVALID_JSON_CHUNK;
        }

        if (offset + glbFile->jsonChunkHeader.chunkLength > size) {
            return GLB_ERROR_INVALID_JSON_CHUNK;
        }

        glbFile->jsonData = (char*)(data + offset);
        offset += glbFile->jsonChunkHeader.chunkLength;

        // Parse binary chunk (if present)
        if (offset < size) {
            if (offset + sizeof(GLBChunkHeader) <= size) {
                memcpy(&glbFile->binChunkHeader, data + offset, sizeof(GLBChunkHeader));
                offset += sizeof(GLBChunkHeader);

                if (glbFile->binChunkHeader.chunkType == GLB_CHUNK_TYPE_BIN &&
                    offset + glbFile->binChunkHeader.chunkLength <= size) {
                    glbFile->binData = (uint8_t*)(data + offset);
                }
            }
        }
        else {
            glbFile->binData = NULL;
            glbFile->binChunkHeader.chunkLength = 0;
            glbFile->binChunkHeader.chunkType = 0;
        }

        return GLB_SUCCESS;
    }

    void freeGLB(GLBFile* glbFile) {
        if (glbFile && glbFile->fileData) {
            free(glbFile->fileData);
            memset(glbFile, 0, sizeof(GLBFile));
        }
    }

}// end namespace glb_loader

#endif // ! GL_LOADER.CPP