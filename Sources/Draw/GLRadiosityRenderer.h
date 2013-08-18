//
//  GLRadiosityRenderer.h
//  OpenSpades
//
//  Created by yvt on 8/12/13.
//  Copyright (c) 2013 yvt.jp. All rights reserved.
//

#pragma once

#include "../Core/Math.h"
#include <vector>
#include "../Core/Debug.h"
#include "IGLDevice.h"
#include <stdint.h>

namespace spades {
	namespace client {
		class GameMap;
	}
	namespace draw {
		class GLRenderer;
		class IGLDevice;
		class GLRadiosityRenderer {
			
			class UpdateDispatch;
			enum {
				ChunkSize = 16,
				ChunkSizeBits = 4,
				Envelope = 6
			};
			GLRenderer *renderer;
			IGLDevice *device;
			client::GameMap *map;
			
			struct Chunk {
				int cx, cy, cz;
				uint16_t dataFlat[ChunkSize][ChunkSize][ChunkSize];
				uint16_t dataX[ChunkSize][ChunkSize][ChunkSize];
				uint16_t dataY[ChunkSize][ChunkSize][ChunkSize];
				uint16_t dataZ[ChunkSize][ChunkSize][ChunkSize];
				bool dirty;
				int dirtyMinX, dirtyMaxX;
				int dirtyMinY, dirtyMaxY;
				int dirtyMinZ, dirtyMaxZ;
				
				volatile bool transfered;
			};
			
			IGLDevice::UInteger textureFlat;
			IGLDevice::UInteger textureX;
			IGLDevice::UInteger textureY;
			IGLDevice::UInteger textureZ;
			
			int w, h, d;
			int chunkW, chunkH, chunkD;
			
			std::vector<Chunk> chunks;
			
			inline Chunk& GetChunk(int cx, int cy, int cz) {
				SPAssert(cx >= 0); SPAssert(cx < chunkW);
				SPAssert(cy >= 0); SPAssert(cy < chunkH);
				SPAssert(cz >= 0); SPAssert(cz < chunkD);
				return chunks[(cx + cy * chunkW) * chunkD + cz];
			}
			
			inline Chunk& GetChunkWrapped(int cx, int cy, int cz) {
				// FIXME: support for non-POT dimensions?
				return GetChunk(cx&(chunkW-1),cy&(chunkH-1),cz);
			}
			
			void Invalidate(int minX, int minY, int minZ,
							int maxX, int maxY, int maxZ);
			
			void UpdateChunk(int cx, int cy, int cz);
			void UpdateDirtyChunks();
			int GetNumDirtyChunks();
			
			UpdateDispatch *dispatch;
		public:
			
			struct Result {
				Vector3 base, x, y, z;
			};
			
			GLRadiosityRenderer(GLRenderer *renderer,
									client::GameMap *map);
			~GLRadiosityRenderer();
			
			Result Evaluate(IntVector3);
			
			void GameMapChanged(int x, int y, int z, client::GameMap *);
			
			void Update();
			
			IGLDevice::UInteger GetTextureFlat() { return textureFlat; }
			IGLDevice::UInteger GetTextureX() { return textureX; }
			IGLDevice::UInteger GetTextureY() { return textureY; }
			IGLDevice::UInteger GetTextureZ() { return textureZ; }
		};
	}
}