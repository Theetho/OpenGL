#include "pch.h"
#include "OBJLoader.h"

Model OBJLoader::LoadFromFile(const std::string & objPath, Loader & loader)
{
	std::ifstream file("res/obj/"+ objPath +".obj");
	std::string line = "";
	// Vectors containing the data
	std::vector<glm::vec3> vertices, normals;
	std::vector<glm::vec2> textures_coordinates;
	std::vector<unsigned int> indices;
	// Table that contains ordered data (because they are not ordered by default)
	float * ordered_textures = nullptr, *ordered_normals = nullptr;
	unsigned v_index[4], t_index[4], n_index[4], count;
	bool vertex_status = true, textures_status = true, normals_status = true;

	if (file.is_open())
	{
		while (std::getline(file, line))
		{
			// Switching state depending on how the line begins
			if (line[0] == 'v')
			{
				if (line[1] == ' ')			// Vertex coordinates
				{
					if (vertex_status)
					{
						std::cout << "Starting the load of the " + objPath + " ...\n";
						vertex_status = false;
					}
					vertices.push_back(glm::vec3());
					sscanf_s(line.c_str(), "v %f %f %f", &vertices.back().x , &vertices.back().y, &vertices.back().z);
				}
				else if (line[1] == 't')	// Texture coordinates 
				{
					if (textures_status)
					{
						std::cout << "Vertices coordinates loaded ...\n";
						textures_status = false;
					}
					textures_coordinates.push_back(glm::vec2());
					sscanf_s(line.c_str(), "vt %f %f", &textures_coordinates.back().x, &textures_coordinates.back().y);
				}
				else if (line[1] == 'n')	// Normal coordinates 
				{
					if (normals_status)
					{
						std::cout << "Textures coordinates loaded ...\n";
						normals_status = false;
					}
					normals.push_back(glm::vec3());
					sscanf_s(line.c_str(), "vn %f %f %f", &normals.back().x, &normals.back().y, &normals.back().z);
				}
			}
			else if (line[0] == 'f')		// Faces
			{				
				/*	Here we have to order the textures coordinates and the normals depending on their associated
					vertex indice, so we create to table, and we can give them a size now that we have all the
					vertices coordinates																		  */
				if (!ordered_textures && !ordered_normals)
				{
					// There are two textures coordinates and three normals / vertices (glm::vec3 for now)
					ordered_textures	= new float[vertices.size() * 2];
					ordered_normals		= new float[vertices.size() * 3];
					std::cout << "Normals coordinates loaded ...\n";
				}
				if (std::count(line.begin(), line.end(), ' ') == 4)
				{
					count = 4;
					sscanf_s(line.c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d", 
							&v_index[0], &t_index[0], &n_index[0],
							&v_index[1], &t_index[1], &n_index[1],
							&v_index[2], &t_index[2], &n_index[2],
							&v_index[3], &t_index[3], &n_index[3] );
				}
				else if (std::count(line.begin(), line.end(), ' ') == 3)
				{
					count = 3;
					sscanf_s(line.c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d",
							&v_index[0], &t_index[0], &n_index[0],
							&v_index[1], &t_index[1], &n_index[1],
							&v_index[2], &t_index[2], &n_index[2]);
				}
				for (unsigned i = 0; i < count; ++i)
				{
					// Add the indice of the vertex
					indices.push_back((unsigned int)(v_index[i] - 1));
					// Then the texture coordinates in the right index in the ordered table
					ordered_textures[(v_index[i] - 1) * 2	 ] = textures_coordinates[t_index[i] - 1].x;
					ordered_textures[(v_index[i] - 1) * 2 + 1] = textures_coordinates[t_index[i] - 1].y;
					// And then the normals
					ordered_normals[(v_index[i] - 1) * 3    ] = normals[n_index[i] - 1].x;
					ordered_normals[(v_index[i] - 1) * 3 + 1] = normals[n_index[i] - 1].y;
					ordered_normals[(v_index[i] - 1) * 3 + 2] = normals[n_index[i] - 1].z;
				}	
			}
		}
		file.close();
		/*	In the end, we have to convert the ordered table into vertor<float> and the vertices into vector<float>
			instead of vector<vec3>																					 */
		std::vector<float> final_vertices, final_textures_coordinates, final_normals;
		for (unsigned j = 0; j < vertices.size(); ++j)
		{
			final_vertices.push_back(vertices[j].x);
			final_vertices.push_back(vertices[j].y);
			final_vertices.push_back(vertices[j].z);

			final_vertices.push_back(ordered_normals[j]);
			final_vertices.push_back(ordered_normals[j + 1]);
			final_vertices.push_back(ordered_normals[j + 2]);

			final_vertices.push_back(ordered_textures[j]);
			final_vertices.push_back(ordered_textures[j + 1]);
		}
		// Finally, we can load all the data in a vao and create a 3D model
		std::cout << "Model loaded ! Sending it to a vao\n";
		return loader.LoadToVao(final_vertices, indices);
	}
	else
	{
		std::cerr << "Can't load the given OBJ file ! Error in file '" << __FILE__ << "', line " << __LINE__ << std::endl;
		return Model(0, 0, false);
	}
}

OBJLoader::OBJLoader()
{
}

OBJLoader::~OBJLoader()
{
}