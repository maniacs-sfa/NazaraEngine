// Copyright (C) 2017 Jérôme Leclercq
// This file is part of the "Nazara Engine - Utility module"
// For conditions of distribution and use, see copyright notice in Config.hpp

#pragma once

#ifndef NAZARA_MESH_HPP
#define NAZARA_MESH_HPP

#include <Nazara/Prerequesites.hpp>
#include <Nazara/Core/ObjectLibrary.hpp>
#include <Nazara/Core/ObjectRef.hpp>
#include <Nazara/Core/Primitive.hpp>
#include <Nazara/Core/RefCounted.hpp>
#include <Nazara/Core/Resource.hpp>
#include <Nazara/Core/ResourceLoader.hpp>
#include <Nazara/Core/ResourceManager.hpp>
#include <Nazara/Core/ResourceParameters.hpp>
#include <Nazara/Core/ResourceSaver.hpp>
#include <Nazara/Core/Stream.hpp>
#include <Nazara/Core/String.hpp>
#include <Nazara/Math/Box.hpp>
#include <Nazara/Utility/Skeleton.hpp>
#include <Nazara/Utility/SubMesh.hpp>
#include <Nazara/Utility/VertexStruct.hpp>

namespace Nz
{
	struct NAZARA_UTILITY_API MeshParams : ResourceParameters
	{
		MeshParams();

		Matrix4f matrix = Matrix4f::Identity();     ///< A matrix which will transform every vertex position
		DataStorage storage = DataStorage_Hardware; ///< The place where the buffers will be allocated
		Vector2f texCoordOffset = {0.f, 0.f};       ///< Offset to apply on the texture coordinates (not scaled)
		Vector2f texCoordScale  = {1.f, 1.f};       ///< Scale to apply on the texture coordinates
		bool animated = true;                       ///< If true, will load an animated version of the model if possible
		bool center = false;                        ///< If true, will center the mesh vertices around the origin
		bool optimizeIndexBuffers = true;           ///< Optimize the index buffers after loading, improve cache locality (and thus rendering speed) but increase loading time.

		bool IsValid() const;
	};

	class Mesh;
	class PrimitiveList;

	typedef VertexStruct_XYZ_Normal_UV_Tangent MeshVertex;
	typedef VertexStruct_XYZ_Normal_UV_Tangent_Skinning SkeletalMeshVertex;

	using MeshConstRef = ObjectRef<const Mesh>;
	using MeshLibrary = ObjectLibrary<Mesh>;
	using MeshLoader = ResourceLoader<Mesh, MeshParams>;
	using MeshManager = ResourceManager<Mesh, MeshParams>;
	using MeshRef = ObjectRef<Mesh>;
	using MeshSaver = ResourceSaver<Mesh, MeshParams>;

	struct MeshImpl;

	class NAZARA_UTILITY_API Mesh : public RefCounted, public Resource
	{
		friend MeshLibrary;
		friend MeshLoader;
		friend MeshManager;
		friend MeshSaver;
		friend class Utility;

		public:
			Mesh() = default;
			~Mesh();

			void AddSubMesh(SubMesh* subMesh);
			void AddSubMesh(const String& identifier, SubMesh* subMesh);

			SubMesh* BuildSubMesh(const Primitive& primitive, const MeshParams& params = MeshParams());
			void BuildSubMeshes(const PrimitiveList& list, const MeshParams& params = MeshParams());

			bool CreateSkeletal(UInt32 jointCount);
			bool CreateStatic();
			void Destroy();

			void GenerateNormals();
			void GenerateNormalsAndTangents();
			void GenerateTangents();

			const Boxf& GetAABB() const;
			String GetAnimation() const;
			AnimationType GetAnimationType() const;
			UInt32 GetJointCount() const;
			ParameterList& GetMaterialData(UInt32 index);
			const ParameterList& GetMaterialData(UInt32 index) const;
			UInt32 GetMaterialCount() const;
			Skeleton* GetSkeleton();
			const Skeleton* GetSkeleton() const;
			SubMesh* GetSubMesh(const String& identifier);
			SubMesh* GetSubMesh(UInt32 index);
			const SubMesh* GetSubMesh(const String& identifier) const;
			const SubMesh* GetSubMesh(UInt32 index) const;
			UInt32 GetSubMeshCount() const;
			UInt32 GetSubMeshIndex(const String& identifier) const;
			UInt32 GetTriangleCount() const;
			UInt32 GetVertexCount() const;

			bool HasSubMesh(const String& identifier) const;
			bool HasSubMesh(UInt32 index = 0) const;

			void InvalidateAABB() const;

			bool IsAnimable() const;
			bool IsValid() const;

			bool LoadFromFile(const String& filePath, const MeshParams& params = MeshParams());
			bool LoadFromMemory(const void* data, std::size_t size, const MeshParams& params = MeshParams());
			bool LoadFromStream(Stream& stream, const MeshParams& params = MeshParams());

			void Recenter();

			void RemoveSubMesh(const String& identifier);
			void RemoveSubMesh(UInt32 index);

			bool SaveToFile(const String& filePath, const MeshParams& params = MeshParams());
			bool SaveToStream(Stream& stream, const String& format, const MeshParams& params = MeshParams());

			void SetAnimation(const String& animationPath);
			void SetMaterialCount(UInt32 matCount);
			void SetMaterialData(UInt32 matIndex, ParameterList data);

			void Transform(const Matrix4f& matrix);

			template<typename... Args> static MeshRef New(Args&&... args);

			// Signals:
			NazaraSignal(OnMeshDestroy, const Mesh* /*mesh*/);
			NazaraSignal(OnMeshRelease, const Mesh* /*mesh*/);

		private:
			MeshImpl* m_impl = nullptr;

			static bool Initialize();
			static void Uninitialize();

			static MeshLibrary::LibraryMap s_library;
			static MeshLoader::LoaderList s_loaders;
			static MeshManager::ManagerMap s_managerMap;
			static MeshManager::ManagerParams s_managerParameters;
			static MeshSaver::SaverList s_savers;
	};
}

#include <Nazara/Utility/Mesh.inl>

#endif // NAZARA_MESH_HPP
