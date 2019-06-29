/**
* @file Mesh.h
*/
#ifndef MESH_H_INCLUDED
#define MESH_H_INCLUDED
#include <GL/glew.h>
#include "BufferObject.h"
#include "Texture.h"
#include "Shader.h"
#include <glm/glm.hpp>
#include <vector>
#include <unordered_map>
#include <string>
#include <memory>

namespace Mesh
{
	//先行宣言
	class Mesh;
	using MeshPtr = std::shared_ptr<Mesh>;
	class MeshBuffer;
	using MeshBufferPtr = std::shared_ptr<Mesh>;

	/**
	* 頂点データ
	*/
	struct Vertex
	{
		glm::vec3 position;
		glm::vec2 texCoord;
		glm::vec3 normal;
	};
	/**
	* プリミティブの材質
	*/
	struct Material
	{
		glm::vec4 baseColor = glm::vec4(1);
		Texture::Image2DPtr texture;
		Shader::ProgramPtr program;
	};
	/**
	* 頂点データの描画パラメータ
	*/
	struct Primitive
	{
		GLenum mode;
		GLsizei count;
		GLenum type;
		const GLvoid* indices;
		GLint baseVertex = 0;
		std::shared_ptr<VertexArrayObject> vao;
		int material = 0;
	};
	/**
	* メッシュ
	*/
	struct Mesh
	{
		std::string name;//メッシュ名
		std::vector<Primitive> primitives;
	};
	/**
	*ファイル
	*/
	struct File
	{
		std::string name;//ファイル名
		std::vector<Mesh> meshes;
		std::vector<Material> materials;
	};
	using FilePtr = std::shared_ptr<File>;

	/**
	*メッシュ管理クラス
	*/
	class Buffer
	{
	public:
		Buffer() = default;
		~Buffer() = default;

		bool Init(GLsizeiptr vboSize, GLsizeiptr iboSize);
		GLintptr AddVertexData(const void* data, size_t size);
		GLintptr AddIndexData(const void* data, size_t size);
		Primitive CreatePrimitive(
			size_t count, GLenum type, size_t iOffset, size_t vOffset) const;
		Material CreateMaterial(const glm::vec4& color, Texture::Image2DPtr texture) const;
		bool AddMesh(const char* name, const Primitive& primitive, const Material& material);
		FilePtr GetFile(const char* name) const;
		void AddCude(const char* name);

	private:
		BufferObject vbo;
		BufferObject ibo;
		GLintptr vboEnd = 0;
		GLintptr iboEnd = 0;
		std::unordered_map<std::string, FilePtr> files;
		Shader::ProgramPtr progStaticMesh;
	};
	void Draw(const FilePtr&, const glm::mat4& matVP, const glm::mat4& matM);
}//namespace Mesh
#endif //MESH_H_INCLUDED