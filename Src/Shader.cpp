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
* コンストラクタ
*/
	Program::Program()
	{
	}
/**
* コンストラクタ
*
* @param programId プログラムオブジェクトのID
*/
	Program::Program(GLuint programId)
	{
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
			GLint locPointLightCount = -1;
			GLint locPointLightIndex = -1;
			GLint locSpotLightCount = -1;
			GLint locSpotLightIndex = -1;
			GLint locCameraPosition = -1;
			GLint locTime = -1;
			return;
		}
		locMatMVP = glGetUniformLocation(id, "matMVP");
		locMatModel = glGetUniformLocation(id, "matModel");
		locPointLightCount = glGetUniformLocation(id, "pointLightCount");
		locPointLightIndex = glGetUniformLocation(id, "pointLightIndex");
		locSpotLightCount = glGetUniformLocation(id, "spotLightCount");
		locSpotLightIndex = glGetUniformLocation(id, "spotLightIndex");
		locCameraPosition = glGetUniformLocation(id, "cameraPosition");
		locTime = glGetUniformLocation(id, "Time");

		glUseProgram(id);
		const GLint texColorLoc = glGetUniformLocation(id, "texColor");
		if (texColorLoc >= 0)
		{
			glUniform1i(texColorLoc, 0);
		}

		for (int i = 0; i < 8; i++)
		{
			std::string name("texColorArray[");
			name += static_cast<char>('0' + i);
			name += ']';
			const GLint texColorLoc = glGetUniformLocation(id, name.c_str());
			if (texColorLoc >= 0)
			{
				glUniform1i(texColorLoc, i);
			}
		}
		for (GLint i = 0; i < 8; i++)
		{
			std::string name("texNormalArray[");
			name += static_cast<char>('0' + i);
			name += ']';
			const GLint texColorLoc = glGetUniformLocation(id, name.c_str());
			if (texColorLoc >= 0)
			{
				glUniform1i(texColorLoc, i + 8);
			}
		}
		const GLint locTexPointLightIndex = glGetUniformLocation(id, "texPointLightIndex");
		if (locTexPointLightIndex >= 0)
		{
			glUniform1i(locTexPointLightIndex, 4);
		}
		const GLint locTexSpotLightIndex = glGetUniformLocation(id, "texSpotLightIndex");
		if (locTexSpotLightIndex >= 0)
		{
			glUniform1i(locTexSpotLightIndex, 5);
		}
		const GLint locTexCubeMap = glGetUniformLocation(id, "texCubeMap");
		if (locTexCubeMap >= 0)
		{
			glUniform1i(locTexCubeMap, 6);
		}

		glUseProgram(0);
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
	* 描画に使われるビュー プロジェクション行列を設定する
	*
	* @param matMVP 設定するビュー プロジェクション行列
	*/
	void Program::SetViewProjectionMatrix(const glm::mat4& matVP)
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
	* @param count 描画に使用するポイントライトの数(0～8)
	* @param indexList 描画に使用するポイントライト番号の配列
	*/
	void Program::SetPointLightIndex(int count, const int* indexList)
	{
		if (locPointLightCount >= 0)
		{
			glUniform1i(locPointLightCount, count);
		}
		if (locPointLightIndex >= 0 && count > 0)
		{
			glUniform1iv(locPointLightIndex, count, indexList);
		}
	}

	/**
	* 描画に使われるライトを設定する
	*
	* @param count 描画に使用するスポットライトの数(0～8)
	* @param indexList 描画に使用するスポットライト番号の配列
	*/
	void Program::SetSpotLightIndex(int count, const int* indexList)
	{
		if (locSpotLightCount >= 0)
		{
			glUniform1i(locSpotLightCount, count);
		}
		if (locSpotLightIndex >= 0 && count > 0)
		{
			glUniform1iv(locSpotLightIndex, count, indexList);
		}
	}

	/**
	* カメラ座標を設定する
	*
	* @param pos カメラ座標
	*/
	void Program::SetCameraPostion(const glm::vec3& pos)
	{
		if (locCameraPosition >= 0)
		{
			glUniform3fv(locCameraPosition, 1, &pos.x);
		}
	}

	/**
	* 経過時間を設定する
	*
	* param time 総経過時間
	*/
	void Program::SetTime(float time)
	{
		if (locTime >= 0)
		{
			glUniform1f(locTime, time);
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
}//namespace Shader
