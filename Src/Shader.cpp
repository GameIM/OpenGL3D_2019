/*
@file Shader.cpp
*/
#include "Shader.h"
#include "Geometry.h"
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <iostream>
#include <fstream>

/*
シェーダーに関する機能を格納する名前空間
*/
namespace Shader
{
	/**
	*シェーダー・プログラムをコンパイルする
	*
	* @param type シェーダーの種類
	* @param string シェーダー・プログラムへのポインタ
	*
	* @retval 0より大きい 作成したシェーダー・オブジェクト
	* @retval 0           シェーダー・オブジェクトの作成に失敗
	*/
	GLuint Compile(GLenum type, const GLchar* string)
	{
		if (!string)
		{
			return 0;
		}
		GLuint shader = glCreateShader(type);
		glShaderSource(shader, 1, &string, nullptr);
		glCompileShader(shader);
		GLint compiled = 0;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
		//コンパイルに失敗した場合、原因をコンソールに出力して0を返す
		if (!compiled)
		{
			GLint infoLen = 0;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
			if (infoLen)
			{
				std::vector<char>buf;
				buf.resize(infoLen);

				if (static_cast<int>(buf.size()) >= infoLen)
				{
					glGetShaderInfoLog(shader, infoLen, NULL, buf.data());
					std::cerr << "ERROR: シェーダーのコンパイルに失敗\n" << buf.data() << std::endl;
				}
			}
			glDeleteShader(shader);
			return 0;
		}
		return shader;
	}


/**
*プログラム・オブジェクトを作成する
*
* @pram vsCode 頂点シェーダー・プログラムへのポインタ
* @param fsCode フラグメントシェーダー・プログラムへのポインタ

* @retval 0より大きい 作成したプログラム・オブジェクト
* @retval 0 プログラム・オブジェクトの作成に失敗
*/
	GLuint Build(const GLchar* vsCode, const GLchar* fsCode)
	{
		GLuint vs = Compile(GL_VERTEX_SHADER, vsCode);
		GLuint fs = Compile(GL_FRAGMENT_SHADER, fsCode);
		if (!vs || !fs)
		{
			return 0;
		}
		GLuint program = glCreateProgram();
		glAttachShader(program, fs);
		glDeleteShader(fs);
		glAttachShader(program, vs);
		glDeleteShader(vs);
		glLinkProgram(program);
		GLint linkStatus = GL_FALSE;
		glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
		if (linkStatus != GL_TRUE)
		{
			GLint infoLen = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLen);
			if (infoLen)
			{
				std::vector<char> buf;
				buf.resize(infoLen);
				if (static_cast<int>(buf.size()) >= infoLen)
				{
					glGetProgramInfoLog(program, infoLen, NULL, buf.data());
					std::cerr << "ERROR: シェーダーのリンクに失敗.\n" << buf.data() << std::endl;
				}
			}
			glDeleteProgram(program);
			return 0;
		}
		return program;
	}

/*
*ファイル読み込み
*
* @param path 読み込みファイル名
*
* @return 読み込んだデータ
*/
	std::vector<GLchar> ReadFile(const char* path)
	{
		std::basic_ifstream<GLchar> ifs;
		ifs.open(path, std::ios_base::binary);
		if (!ifs.is_open())
		{
			std::cerr << "ERROR:" << path << "を開けません.\n";
			return {};
		}
		ifs.seekg(0, std::ios_base::end);
		const size_t length = (size_t)ifs.tellg();
		ifs.seekg(0, std::ios_base::beg);
		std::vector<GLchar> buf(length);
		ifs.read(buf.data(), length);
		buf.push_back('\0');
		return buf;
	}
/**
*ファイルからプログラム・オブジェクトを作成する
*
* @param vspath 頂点シェーダー・ファイル名
* @param fspath フラグメントシェーダー・ファイル名
*
*　@return 作成したプログラム・オブジェクト
*/
	GLuint BuildFromFile(const char* vspath, const char* fspath)
	{
		const std::vector<GLchar> vsCode = ReadFile(vspath);
		const std::vector<GLchar> fsCode = ReadFile(fspath);
		return Build(vsCode.data(), fsCode.data());
	}
/**
* ライトリストを初期化する
*
* 全ての光源の明るさを0にする
*/
	void LightList::Init()
	{
		ambient.color = glm::vec3(0);
		directional.color = glm::vec3(0);
		for (int i = 0; i < 8; ++i)
		{
			point.color[i] = glm::vec3(0);
		}
		for (int i = 0; i < 4; ++i)
		{
			spot.color[i] = glm::vec3(0);
		}
	}
/**
* コンストラクタ
*/
	Program::Program()
	{
		lights.Init();
	}
/**
* コンストラクタ
*
* @param programId プログラムオブジェクトのID
*/
	Program::Program(GLuint programId)
	{
		lights.Init();
		Reset(programId);
	}

/**
* デストラクタ
*
* プログラム オブジェクトを削除する
*/
	Program::~Program()
	{
		if (id)
		{
			glDeleteProgram(id);
		}
	}
/**
* プログラム オブジェクトのID
*
* @param id プログラム オブジェクトのID
*/
	void Program::Reset(GLuint programId)
	{
		glDeleteProgram(id);
		id = programId;
		if (id == 0)
		{
			locMatMVP = -1;
			locMatModel = -1;
			locPointLightPos = -1;
			locPointLightCol = -1;
			locDirLightDir = -1;
			locDirLightCol = -1;
			locAmbLightCol = -1;
			locSpotLightPos = -1;
			locSpotLightDir = -1;
			locSpotLightCol = -1;
			return;
		}
		locMatMVP = glGetUniformLocation(id, "matMVP");
		locMatModel = glGetUniformLocation(id, "matModel");
		locPointLightPos = glGetUniformLocation(id, "pointLight.position");
		locPointLightCol = glGetUniformLocation(id, "pointLight.color");
		locDirLightDir = glGetUniformLocation(id, "directionalLight.direction");
		locDirLightCol = glGetUniformLocation(id, "directionalLight.color");
		locAmbLightCol = glGetUniformLocation(id, "ambientLight.color");
		locSpotLightPos = glGetUniformLocation(id, "spotLight.posAndInnerCutOff");
		locSpotLightDir = glGetUniformLocation(id, "spotLight.dirAndCutOff");
		locSpotLightCol = glGetUniformLocation(id, "spotLight.color");

		const GLint texColorLoc = glGetUniformLocation(id, "texColor");
		if (texColorLoc >= 0)
		{
			glUseProgram(id);
			glUniform1i(texColorLoc, 0);
			glUseProgram(0);
		}
	}
/**
* プログラム オブジェクトが設定されているか調べる
*
* @retval true 設定されていない
* @retval false 設定されている
*/
	bool Program::IsNull() const
	{
		return id == 0;
	}


	/**
	* プログラム オブジェクトをグラフィック パイプラインに割り当てる
	*/
	void Program::Use()
	{
		if (id)
		{
			glUseProgram(id);
		}
	}

	/**
	* 描画に使用するテクスチャを設定する
	*
	* @param unitNo 設定するテクスチャ イメージユニットの番号(0～)
	* @param texId 設定するテクスチャのID
	*/
	void Program::BindTexture(GLuint unitNo, GLuint texId)
	{
		glActiveTexture(GL_TEXTURE0 + unitNo);
		glBindTexture(GL_TEXTURE_2D, texId);
	}

	/**
	* 描画に使われるライトを設定する
	*
	* @param lights 設定するライト
	*/
	void Program::SetLightList(const LightList& lights)
	{
		this->lights = lights;

		//ライトの色情報をGPUメモリに転送する
		if (locAmbLightCol >= 0)
		{
			glUniform3fv(locAmbLightCol, 1, &lights.ambient.color.x);
		}
		if (locDirLightCol >= 0)
		{
			glUniform3fv(locDirLightCol, 1, &lights.directional.color.x);
		}
		if (locPointLightCol >= 0)
		{
			glUniform3fv(locPointLightCol, 8, &lights.point.color[0].x);
		}
		if (locSpotLightCol >= 0)
		{
			glUniform3fv(locSpotLightCol, 4, &lights.spot.color[0].x);
		}
	}

	/**
	* 描画に使われるビュー プロジェクション行列を設定する
	*
	* @param matMVP 設定するビュー プロジェクション行列
	*/
	void Program::SetViewprojectionMatrix(const glm::mat4& matVP)
	{
		this->matVP = matVP;
		if (locMatMVP >= 0)
		{
			glUniformMatrix4fv(locMatMVP, 1, GL_FALSE, &matVP[0][0]);
		}
	}

	/**
	* 描画に使われるモデル行列を設定する
	*
	* @param m 設定するモデル行列
	*/
	void Program::SetModelMatrix(const glm::mat4& m)
	{
		if (locMatModel >= 0)
		{
			glUniformMatrix4fv(locMatModel, 1, GL_FALSE, &m[0][0]);
		}
	}

	/**
	* プログラムオブジェクトを作成する
	*
	* @param vsPath 頂点シェーダーファイル名
	* @param fsPath フラグメントシェーダーファイル名
	*
	* @return 作成したプログラムオブジェクト
	*/
	ProgramPtr Program::Create(const char* vsPath, const char* fsPath)
	{
		return std::make_shared<Program>(BuildFromFile(vsPath, fsPath));
	}
}//Shader namespace
