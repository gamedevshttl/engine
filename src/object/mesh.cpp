#include "mesh.h"
#include <iostream>

#include "../texture_logic.h"

mesh::mesh(const std::vector<vertex>& verteces, const std::vector<GLuint>& indices, const std::vector<texture>& textures)
	: m_verteces(verteces)
	, m_indices(indices)
	, m_textures(textures)
{
	setup_mesh();
}

void mesh::setup_mesh()
{
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_EBO);

	glBindVertexArray(m_VAO);
	
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, m_verteces.size() * sizeof(vertex), &m_verteces[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(GLuint), &m_indices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, m_normal));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, m_tex_coords));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void mesh::draw(shader_logic& shader)
{
	GLuint diffuse_nr = 1;
	GLuint specular_nr = 1;

	for (GLuint i = 0; i < m_textures.size(); ++i) {
		glActiveTexture(GL_TEXTURE0 + i);

		std::string number;
		std::string name = m_textures[i].m_type;
		if (name == "texture_diffuse")
			number = std::to_string(diffuse_nr++);
		else if(name == "texture_specular")
			number = std::to_string(specular_nr++);

		shader.set_int(name + number, i);

		glBindTexture(GL_TEXTURE_2D, m_textures[i].m_id);
	}
	
	glBindVertexArray(m_VAO);	
	glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}