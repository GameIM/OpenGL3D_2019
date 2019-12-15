/**
* @file Shader.h
*/
#ifndef SHADER_H_INCLUDED
#define SHADER_H_INCLUDED
#include <GL/glew.h>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <memory>

namespace Shader
{
	class Program;
	using ProgramPtr = std::shared_ptr<Program>;
	GLuint Build(const GLchar* vsCode, const GLchar* fsCode);
	GLuint BuildFromFile(const char* vspath, const char* fspath);

/**
*シェーダープログラム
*/
	class Program
	{
	public:
		Program();
		static ProgramPtr Create(const char* vsPath, const char* fsPath);
		explicit Program(GLuint programId);
		~Program();

		void Reset(GLuint program);
		bool IsNull() const;
		void Use();
		void BindTexture(GLuint, GLuint);
		void SetViewProjectionMatrix(const glm::mat4&);
		void SetModelMatrix(const glm::mat4&);
		void SetPointLightIndex(int count, const int* indexList);
		void SetSpotLightIndex(int count, const int* indexList);
		void SetCameraPostion(const glm::vec3&);
		void SetTime(float);

		//プログラムIDを取得する
		GLuint Get() const { return id; }

	private:
		GLuint id = 0;//プログラムID

		//uniform変数の位置
		GLint locMatMVP = -1;
		GLint locMatModel = -1;
		GLint locPointLightCount = -1;
		GLint locPointLightIndex = -1;
		GLint locSpotLightCount = -1;
		GLint locSpotLightIndex = -1;
		GLint locCameraPosition = -1;
		GLint locTime = -1;

		glm::mat4 matVP = glm::mat4(1);//ビュープロジェクト行列
	};
} //Shader namespace
#endif //SHADER_H_INCLUDED