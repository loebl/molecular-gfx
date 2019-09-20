/*	MaterialManager.h

MIT License

Copyright (c) 2019 Fabian Herb

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef MOLECULAR_MATERIALMANAGER_H
#define MOLECULAR_MATERIALMANAGER_H

#include <molecular/util/PointerContainer.h>
#include <molecular/util/IniFile.h>
#include <molecular/util/MtlFile.h>
#include <molecular/util/Hash.h>
#include "TextureManager.h"
#include "Scoping.h"

namespace molecular
{
namespace Gfx
{

// Includes not possible here due to circular dependency
class Material;

/// Loads materials from ini files and provides them on request
class MaterialManager
{
public:
	template<class TRenderManager>
	MaterialManager(TRenderManager& renderManager)  : mTextureManager(renderManager.GetTextureManager()), mScoping(renderManager.GetScoping()) {}

	MaterialManager(TextureManager& textureManager, Gfx::Scoping& scoping) : mTextureManager(textureManager), mScoping(scoping) {}
	~MaterialManager();

	template<class TStorage>
	void ReadIniFile(TStorage& file);

	template<class TStorage>
	void ReadMtlFile(TStorage& file);

	void ReadIniFile(const Blob& blob);

	void ReadMtlFile(const Blob& blob);

	void ReadFile(IniFile& file);

	void ReadFile(MtlFile& file);

	Material* GetMaterial(Hash hash) const;
	Material* GetMaterial(const std::string& material) const;

private:
	void AddVariable(Material* material, Hash key, const std::string& value);

	TextureManager& mTextureManager;
	Gfx::Scoping& mScoping;

	std::unordered_map<Hash, std::unique_ptr<Material>> mMaterials;
};


template<class TStorage>
void MaterialManager::ReadIniFile(TStorage& storage)
{
	IniFile file(storage);
	ReadFile(file);
}

template<class TStorage>
void MaterialManager::ReadMtlFile(TStorage& file)
{
	size_t size = file.GetSize();
	std::vector<char> buffer(size);
	file.Read(buffer.data(), size);
	MtlFile mtlFile(buffer.data(), buffer.data() + size);
	ReadFile(mtlFile);
}

}
}

#endif // MOLECULAR_MATERIALMANAGER_H
