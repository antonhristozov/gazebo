/*
 * Copyright 2012 Open Source Robotics Foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
*/

#ifndef _DARTRAYSHAPE_HH_
#define _DARTRAYSHAPE_HH_

#include <string>

#include "gazebo/physics/RayShape.hh"
#include "gazebo/physics/Shape.hh"

#include "gazebo/physics/dart/DARTTypes.hh"

namespace gazebo
{
  namespace physics
  {
    /// \brief Ray collision
    class DARTRayShape : public RayShape
    {
      /// \brief Constructor for a global ray.
      /// \param[in] _physicsEngine Pointer to the physics engine.
      public: explicit DARTRayShape(PhysicsEnginePtr _physicsEngine);

      /// \brief Constructor.
      /// \param[in] _collision Collision object this ray is attached to.
      public: explicit DARTRayShape(CollisionPtr _collision);

      /// \brief Destructor.
      public: virtual ~DARTRayShape();

      public: virtual void Update();

      public: virtual void GetIntersection(double &_dist, std::string &_entity);

      public: virtual void SetPoints(const math::Vector3 &_posStart,
                                     const math::Vector3 &_posEnd);

      /// \brief Ray-intersection callback.
      /// \param[in] _data Pointer to user data.
      /// \param[in] _o1 First geom to check for collisions.
      /// \param[in] _o2 Second geom to check for collisions.
//       private: static void UpdateCallback(void *_data, dGeomID _o1,
//                                           dGeomID _o2);

      /// \brief DART geom id.
      //private: dGeomID geomId;

      /// \brief Pointer to the DART physics engine
      private: DARTPhysicsPtr physicsEngine;

      /// \brief An intersection class keeping track of name and depth of
      ///        intersections.
//       private: class Intersection
//                {
//                  /// \brief Depth of the ray intersection.
//                  public: double depth;
// 
//                  /// \brief Name of the collision object that was hit.
//                  public: std::string name;
//                };
    };
  }
}
#endif
