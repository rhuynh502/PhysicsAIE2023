#include "SoftBody.h"
#include "Circle.h"
#include "Spring.h"

void SoftBody::Build(PhysicsScene* _scene, glm::vec2 _pos, float _damping, float _springForce, float _spacing, std::vector<std::string>& _strings)
{
	int numCols = _strings.size();
	int numRows = _strings[0].length();

	Circle** circles = new Circle * [numRows * numCols];
	// spawn circles
	for (int i = 0; i < numRows; i++)
	{
		for (int j = 0; j < numCols; j++)
		{
			if (_strings[j][i] == '0')
			{
				circles[i * numCols + j] = new Circle(_pos + glm::vec2(i, j) * _spacing, glm::vec2(0), 1.0f, 1.0f, glm::vec4(1, 0, 1, 1));
				_scene->AddActor(circles[i * numCols + j]);
			}
			else
			{
				circles[i * numCols + j] = nullptr;
			}

		}
	}

	// spawn springs
	for (int i = 1; i < numRows; i++)
	{
		for (int j = 1; j < numCols; j++)
		{
			Circle* s11 = circles[i * numCols + j];
			Circle* s01 = circles[(i - 1) * numCols + j];
			Circle* s10 = circles[i * numCols + j - 1];
			Circle* s00 = circles[(i - 1) * numCols + j - 1];

			if (s11 && s01 && glm::distance(s11->GetPos(), s01->GetPos()) <= _spacing)
				_scene->AddActor(new Spring(s11, s01, _springForce, _damping, glm::vec4(1, 1, 1, 1), _spacing));
			if (s11 && s10 && glm::distance(s11->GetPos(), s10->GetPos()) <= _spacing)
				_scene->AddActor(new Spring(s11, s10, _springForce, _damping, glm::vec4(1, 1, 1, 1), _spacing));
			if (s10 && s00 && glm::distance(s10->GetPos(), s00->GetPos()) <= _spacing)
				_scene->AddActor(new Spring(s10, s00, _springForce, _damping, glm::vec4(1, 1, 1, 1), _spacing));
			if (s01 && s00 && glm::distance(s01->GetPos(), s00->GetPos()) <= _spacing)
				_scene->AddActor(new Spring(s01, s00, _springForce, _damping, glm::vec4(1, 1, 1, 1), _spacing));

			if (s11 && s00)
				_scene->AddActor(new Spring(s11, s00, _springForce, _damping, glm::vec4(1, 1, 1, 1), _spacing * glm::sqrt(2)));
			if (s01 && s10)
				_scene->AddActor(new Spring(s01, s10, _springForce, _damping, glm::vec4(1, 1, 1, 1), _spacing * glm::sqrt(2)));

			bool endOfJ = j == numCols - 1;
			bool endOfI = i == numRows - 1;

			Circle* s22 = (!endOfI && !endOfJ) ? circles[(i + 1) * numCols + (j + 1)] : nullptr;
			Circle* s02 = !endOfJ ? circles[(i - 1) * numCols + (j + 1)] : nullptr;
			Circle* s20 = !endOfI ? circles[(i + 1) * numCols + j - 1] : nullptr;

			if(s00 && s02)
				_scene->AddActor(new Spring(s00, s02, _springForce, _damping, glm::vec4(1, 1, 1, 1), _spacing * 2));
			if(s00 && s20)
				_scene->AddActor(new Spring(s00, s20, _springForce, _damping, glm::vec4(1, 1, 1, 1), _spacing * 2));
			if(s22 && s02)
				_scene->AddActor(new Spring(s22, s02, _springForce, _damping, glm::vec4(1, 1, 1, 1), _spacing * 2));
			if(s22 && s20)
				_scene->AddActor(new Spring(s22, s20, _springForce, _damping, glm::vec4(1, 1, 1, 1), _spacing * 2));
		}
	}
}
