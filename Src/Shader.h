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

	//環境光
	struct AmbientLight
	{
		glm::vec3 color;
	};

	//指向性ライト
	struct DirectionalLight
	{
		glm::vec3 direction;
		glm::vec3 color;
	};

	//ポイントライト
	struct PointLight
	{
		glm::vec3 position[8];
		glm::vec3 color[8];
	};

	//スポットライト
	struct SpotLight
	{
		glm::vec4 dirAndCutOff[4];//光の方向(xyzに入れる)とcos(放射角)(wに入れる)
		glm::vec4 posAndInnerCutOff[4];//光の位置(xyzに入れる)とcos(減衰開始角)(wに入れる)
		glm::vec3 color[4];
	};

/**
* ライトをまとめた構造体
*/
	struct LightList
	{
		AmbientLight ambient;
		DirectionalLight directional;
		PointLight point;
		SpotLight spot;

		void Init();
	};
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
		void SetLightList(const LightList&);
		void SetViewprojectionMatrix(const glm::mat4&);
		void SetModelMatrix(const glm::mat4&);
	private:
		GLuint id = 0;//プログラムID

		//uniform変数の位置
		GLint locMatMVP = -1;
		GLint locMatModel = -1;
		GLint locAmbLightCol = -1;
		GLint locDirLightDir = -1;
		GLint locDirLightCol = -1;
		GLint locPointLightPos = -1;
		GLint locPointLightCol = -1;
		GLint locSpotLightDir = -1;
		GLint locSpotLightPos = -1;
		GLint locSpotLightCol = -1;

		glm::mat4 matVP = glm::mat4(1);//ビュープロジェクト行列
		LightList lights;
	};
} //Shader namespace
#endif //SHADER_H_INCLUDED