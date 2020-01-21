/**
* @file Particle.cpp
*/
#include "Particle.h"
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <algorithm>
#include <random>
#include <numeric>
#include <iostream>
#include <time.h>

namespace /* unname*/
{
	//�p�[�e�B�N���p�̒��_�f�[�^�^
	struct Vertex
	{
		glm::vec3 center;//���S�̍��W
		glm::vec4 color;//�F
		glm::vec2 texCoord;//�e�N�X�`�����W
		glm::vec2 offset;//���S����̋���
	};

	//�p�[�e�B�N���p�̗����G���W��
	std::mt19937 randamEngine(static_cast<int>(time(nullptr)));

	/**
	* int�^�̗����𐶐�����
	*
	* @param min �������闐���̍ŏ��l
	* @param max �������闐���̍ő�l
	*/
	int RandomInt(int min, int max)
	{
		return std::uniform_int_distribution<>(min, max)(randamEngine);
	}

	/**
	* float�^�̗����𐶐�����
	*
	* @param min �������闐���̍ŏ��l
	* @param max �������闐���̍ő�l
	*/
	float RandamFloat(float min, float max)
	{
		return std::uniform_real_distribution<float>(min, max)(randamEngine);
	}
}// unnamed namespace

/**
* �R���X�g���N�^
*
* @param pp �p�[�e�B�N���̃p�����[�^
* @param pos �p�[�e�B�N���̏������W
* @param r �p�[�e�B�N���̃e�N�X�`���\���͈͂�������`
*/
Particle::Particle(const ParticleParameter& pp, const glm::vec3& pos, const Rect& r) :
	rect(r),
	position(pos),
	lifetime(pp.lifetime),
	velocity(pp.velocity),
	acceleration(pp.acceleration),
	scale(pp.scale),
	rotation(pp.rotation),
	color(pp.color)
{

}

/**
* �p�[�e�B�N���̏�Ԃ��X�V����
*
* @param deltatime �O��̍X�V����̌o�ߎ���(�b)
*/
void Particle::Update(float deltatime)
{
	lifetime -= deltatime;
	velocity += acceleration * deltatime;
	position += velocity * deltatime;
}

/**
* �R���X�g���N�^
*
* @param ep �G�~�b�^�[�̏������p�����[�^
* @param pp �p�[�e�B�N���̏������p�����[�^
*/
ParticleEmitter::ParticleEmitter(
	const ParticleEmitterParameter& ep, const ParticleParameter& pp) :
	ep(ep),
	pp(pp),
	interval(1.0f / ep.emissionsPerSecond)
{
	texture = Texture::Image2D::Create(ep.imagePath.c_str());
}

/**
* �p�[�e�B�N����ǉ�����
*/
void ParticleEmitter::AddParticle()
{
	//���o�ʒu�Ƒ��x�����߂邽�߂̍s����v�Z
	const float rx = std::sqrt(RandamFloat(0,1));//X�������̕��o�ʒu��0�`1�͈̔͂ŕ\�����l
	const float ry = RandamFloat(0, glm::two_pi<float>());//0�`360�x�͈̔͂�Y����]������l
	const glm::mat4 matRY = glm::rotate(glm::mat4(1), ry, glm::vec3(0, 1, 0));

	//X -> Z > Y�̏��ŃG�~�b�^�[�̉�]��K�p
	const glm::mat4 matRot = glm::rotate(glm::rotate(glm::rotate(glm::mat4(1),
		ep.rotation.y, glm::vec3(0, 1, 0)),
		ep.rotation.z, glm::vec3(0, 0, -1)),
		ep.rotation.x, glm::vec3(1, 0, 0));

	//�x���V�e�B�����ύX�����p�[�e�B�N���̏������p�p�����[�^�[���쐬
	ParticleParameter tmpPP = pp;
	const glm::mat4 matAngle = glm::rotate(matRY, rx * ep.angle, glm::vec3(0, 0, -1));
	tmpPP.velocity = matRot * matAngle * glm::vec4(pp.velocity, 1);

	//�e�N�X�`���̕\���͈͂��쐬
	Rect rect;
	rect.origin = glm::vec2(0);
	if (ep.titles.x > 1)
	{
		const int tx = RandomInt(0, ep.titles.x - 1);
		rect.origin.x = static_cast<float>(tx) / static_cast<float>(ep.titles.x);
	}
	if (ep.titles.y > 1)
	{
		const int ty = RandomInt(0, ep.titles.y - 1);
		rect.origin.y = static_cast<float>(ty) / static_cast<float>(ep.titles.y);
	}
	rect.size = glm::vec2(1) / glm::vec2(ep.titles);

	const glm::vec3 offset = matRot * matRY * glm::vec4(rx * ep.radius, 0, 0, 1);
	Particle p(tmpPP, ep.position + offset, rect);
	particles.push_back(p);
}

/**
* �G�~�b�^�\�̊Ǘ����ɂ���p�[�e�B�N���̏�Ԃ��X�V����
*
* @param deltatime �O��̍X�V����̌o�ߎ���
*/
void ParticleEmitter::Update(float deltaTime)
{
	//�^�C�}�[�̍X�V
	timer += deltaTime;
	if (timer >= ep.duration)
	{
		if (ep.loop)
		{
			timer -= ep.duration;
			emissionTimer -= ep.duration;
		}
		else
		{
			timer = ep.duration;
		}
	}

	//�p�[�e�B�N������o
	for (; timer - emissionTimer >= interval; emissionTimer += interval)
	{
		AddParticle();
	}

	//�p�[�e�B�N�����X�V
	for (auto& e : particles)
	{
		e.acceleration.y -= ep.gravity * deltaTime;
		e.Update(deltaTime);
	}

	//���S�����p�[�e�B�N�����폜
	particles.remove_if([](const Particle& p) { return p.IsDead(); });
}

/**
* �G�~�b�^�[�̊Ǘ����ɂ���p�[�e�B�N����`�悷��
*/
void ParticleEmitter::Draw()
{
	if (count)
	{
		glBlendFunc(ep.srcFactor, ep.dstFactor);
		glBindTexture(GL_TEXTURE_2D, texture->Get());
		glDrawElementsBaseVertex(GL_TRIANGLES, count, GL_UNSIGNED_SHORT,
			reinterpret_cast<const GLvoid*>(0), baseVertex);
	}
}

/**
* �p�[�e�B�N���V�X�e��������������
*
* @param maxParticleCount �\���\�ȃp�[�e�B�N���̍ő吔
*/
bool ParticleSystem::Init(size_t maxParticleCount)
{
	if (!vbo.Create(GL_ARRAY_BUFFER, sizeof(Vertex) * maxParticleCount * 4,
		nullptr, GL_STREAM_DRAW))
	{
		std::cerr << "[�G���[]�p�[�e�B�N���V�X�e���̏������Ɏ��s\n";
		return false;
	}

	static const GLushort baseIndices[] = { 0,1,2,2,3,0 };
	std::vector<short> indices;
	indices.reserve(4000);
	for (int baseIndex = 0; baseIndex <= std::numeric_limits<GLshort>::max() - 3;
		baseIndex += 4)
	{
		for (auto i : baseIndices)
		{
			indices.push_back(static_cast<GLushort>(baseIndex + i));
		}
	}
	if (!ibo.Create(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * indices.size(),
		indices.data(), GL_STATIC_DRAW))
	{
		std::cerr << "[�G���[]�p�[�e�B�N���V�X�e���̏������Ɏ��s\n";
		return false;
	}

	if (!vao.Create(vbo.Id(), ibo.Id()))
	{
		std::cerr << "[�G���[]�p�[�e�B�N���V�X�e�����������Ɏ��s\n";
		return false;
	}

	vao.Bind();
	vao.VertexAttribPointer(
		0,3, GL_FLOAT, GL_FALSE, sizeof(Vertex), offsetof(Vertex, center));
	vao.VertexAttribPointer(
		1,4, GL_FLOAT, GL_FALSE, sizeof(Vertex), offsetof(Vertex, color));
	vao.VertexAttribPointer(
		2,2, GL_FLOAT, GL_FALSE, sizeof(Vertex), offsetof(Vertex, texCoord));
	vao.VertexAttribPointer(
		3,2, GL_FLOAT, GL_FALSE, sizeof(Vertex), offsetof(Vertex, offset));
	vao.Unbind();
	const GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		std::cerr << "[�G���[]�p�[�e�B�N���V�X�e���̏������Ɏ��s(" <<
			std::hex << error << ")\n";
		return false;
	}

	program = Shader::Program::Create("Res/Particle.vert", "Res/Sprite.frag");
	if (!program)
	{
		std::cerr << "[�G���[]�p�[�e�B�N���V�X�e���̏������Ɏ��s\n";
		return false;
	}
	return true;
}

/**
* �G�~�b�^�[��ǉ�����
*
* @param ep �G�~�b�^�[�̏������p�����[�^
* @param pp �G�~�b�^�[�̏������p�����[�^
*
* @return �ǉ������G�~�b�^�[
*/
ParticleEmitterPtr ParticleSystem::Add(
	const ParticleEmitterParameter& ep, const ParticleParameter& pp)
{
	ParticleEmitterPtr p = std::make_shared<ParticleEmitter>(ep, pp);
	emitters.push_back(p);
	return p;
}

/**
* �w�肳�ꂽID�����G�~�b�^�[����������
*
* @param id ��������ID
*
* @retval nullptr�ȊO ����id�ƈ�v����ID�����G�~�b�^�\
* @retval nullptr ��v����G�~�b�^�[�͑��݂��Ȃ�
*/
ParticleEmitterPtr ParticleSystem::Find(int id) const
{
	auto itr = std::find_if(emitters.begin(), emitters.end(),
		[id](const ParticleEmitterPtr& p) { return p->ep.id == id; });
	if (itr != emitters.end())
	{
		return *itr;
	}
	return nullptr;
}

/**
* �w�肳�ꂽ�G�~�b�^�[���폜����
*/
void ParticleSystem::Remove(const ParticleEmitterPtr& p)
{
	emitters.remove(p);
}

/**
* �S�ẴG�~�b�^�[���폜����
*/
void ParticleSystem::Clear()
{
	emitters.clear();
}

/**
* �p�[�e�B�N���̏�Ԃ��X�V����
*
* @param deltaTime �O��̍X�V����̌o�ߎ���(�b)
*/
void ParticleSystem::Update(float deltaTime)
{
	//�G�~�b�^�\�̍X�V�ƍ폜
	for (auto& e : emitters)
	{
		e->Update(deltaTime);
	}
	emitters.remove_if ([](const ParticleEmitterPtr& e) {return e->IsDead(); });

	//�S�ẴG�~�b�^�[�̃p�[�e�B�N���̒��_�f�[�^���쐬
	std::vector<Vertex> vertices;
	vertices.reserve(10000);
	for (auto e : emitters)
	{
		e->baseVertex = vertices.size();
		e->count = 0;
		for (auto& particle : e->particles)
		{
			//���W�ϊ��s����쐬
			const glm::mat4 matR =
				glm::rotate(glm::mat4(1), particle.rotation, glm::vec3(0, 0, 1));
			const glm::mat4 matS = glm::scale(glm::mat4(1), glm::vec3(particle.scale, 1));
			const glm::mat4 transform = matR * matS;

			const Rect& rect = particle.rect;//�ǂ݂₷�����邽�߂̎Q�Ƃ��`

			Vertex v[4];

			v[0].center = particle.position;
			v[0].color = particle.color;
			v[0].texCoord = rect.origin;
			v[0].offset = transform * glm::vec4(-1, -1, 0, 1);

			v[1].center = particle.position;
			v[1].color = particle.color;
			v[1].texCoord = glm::vec2(rect.origin.x + rect.size.x, rect.origin.y);
			v[0].offset = transform * glm::vec4(1, -1, 0, 1);

			v[2].center = particle.position;
			v[2].color = particle.color;
			v[2].texCoord = rect.origin + rect.size;
			v[2].offset = transform * glm::vec4(1, 1, 0, 1);

			v[3].center = particle.position;
			v[3].color = particle.color;
			v[3].texCoord = glm::vec2(rect.origin.x,rect.size.y + rect.origin.y);
			v[3].offset = transform * glm::vec4(-1, -1, 0, 1);

			vertices.insert(vertices.end(), v, v + 4);
			e->count += 6;
		}
	}
	vbo.BufferSubData(0, vertices.size() * sizeof(Vertex), vertices.data());
}

/**
* �p�[�e�B�N����`�悷��
*
* @param matProj �`��Ɏg�p����v���W�F�N�V�����s��
* @param matView �`��Ɏg�p����r���[�s��
*/
void ParticleSystem::Draw(const glm::mat4& matProj, const glm::mat4& matView)
{
	vao.Bind();

	//�`��ݒ�
	glEnable(GL_DEPTH_TEST);
	glDepthMask(false);//�[�x�o�b�t�@�ւ̏������݂��~�߂�
	glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//�V�F�[�_�[�Ƀp�����[�^�[��ݒ�
	program->Use();
	program->SetInverseViewRotationMatrix(matView);
	program->SetViewProjectionMatrix(matProj * matView);
	glActiveTexture(GL_TEXTURE0);

	//�G�~�b�^�[���J�������牓�����ɕ��ׂ�
	struct A
	{
		float z;
		ParticleEmitterPtr p;
	};
	std::vector<A> sortedList;
	sortedList.reserve(emitters.size());
	for (auto& e : emitters)
	{
		const glm::vec3 pos = matView * glm::vec4(e->Position(), 1);
		sortedList.push_back({ pos.z, e });
	}
	std::sort(sortedList.begin(), sortedList.end(),
		[](const A& a, const A& b) {return a.z < b.z; });

	//�G�~�b�^�[��`��
	for (auto& e : sortedList)
	{
		e.p->Draw();
	}

	//�`��ݒ�̌�n��
	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);
	glDepthMask(true);//�[�x�o�b�t�@�ւ̏������݂�����

	vao.Unbind();
}