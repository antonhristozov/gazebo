/*
 * Copyright 2011 Nate Koenig & Andrew Howard
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
/* Desc: A camera sensor using OpenGL
 * Author: Nate Koenig
 * Date: 15 July 2003
 */

#include <sstream>

#include "physics/World.hh"

#include "common/Events.hh"
#include "common/Exception.hh"

#include "transport/transport.h"

#include "rendering/DepthCamera.hh"
#include "rendering/Scene.hh"
#include "rendering/Rendering.hh"

#include "sensors/SensorFactory.hh"
#include "sensors/DepthCameraSensor.hh"

using namespace gazebo;
using namespace sensors;

GZ_REGISTER_STATIC_SENSOR("depth", DepthCameraSensor)
 
//////////////////////////////////////////////////
// Constructor
DepthCameraSensor::DepthCameraSensor()
    : Sensor()
{
}

//////////////////////////////////////////////////
// Destructor
DepthCameraSensor::~DepthCameraSensor()
{
}

//////////////////////////////////////////////////
/// Set the parent of the sensor
void DepthCameraSensor::SetParent( const std::string &_name )
{
  Sensor::SetParent(_name);
}

//////////////////////////////////////////////////
// Load the camera with SDF parameters
void DepthCameraSensor::Load( sdf::ElementPtr &_sdf )
{
  Sensor::Load(_sdf);
}

//////////////////////////////////////////////////
/// Load the camera using default parameters
void DepthCameraSensor::Load()
{
  Sensor::Load();
  this->poseSub = this->node->Subscribe("~/pose", 
                                        &DepthCameraSensor::OnPose, this );
}
 
//////////////////////////////////////////////////
// Initialize the camera
void DepthCameraSensor::Init()
{
  std::string worldName = this->sdf->GetWorldName();

  if (!worldName.empty())
  {
    this->scene = rendering::get_scene(worldName);

    if (!this->scene)
      this->scene = rendering::create_scene(worldName, false);

    this->camera = this->scene->CreateDepthCamera(
        this->sdf->GetValueString("name"),false);

    if (!this->camera)
    {
      gzerr << "Unable to create depth camera sensor\n";
      return;
    }
    this->camera->SetCaptureData(true);

    sdf::ElementPtr cameraSdf = this->sdf->GetOrCreateElement("camera");
    this->camera->Load( cameraSdf );

    // Do some sanity checks
    if (this->camera->GetImageWidth() == 0 || 
        this->camera->GetImageHeight() == 0)
    {
      gzthrow("image has zero size");
    }

    this->camera->Init();
    this->camera->CreateRenderTexture(this->GetName() + "_RttTex_Image");
    this->camera->CreateDepthTexture(this->GetName() + "_RttTex_Depth");
    this->camera->SetWorldPose( this->pose );
    this->camera->AttachToVisual( this->parentName, true );
  }
  else
    gzerr << "No world name\n";

  Sensor::Init();
}

//////////////////////////////////////////////////
// Finalize the camera
void DepthCameraSensor::Fini()
{
  Sensor::Fini();
  this->camera->Fini();
  this->camera.reset();
  this->scene.reset();
}

//////////////////////////////////////////////////
/// Set whether the sensor is active or not
void DepthCameraSensor::SetActive(bool value)
{
  Sensor::SetActive(value);
}

//////////////////////////////////////////////////
// Update the drawing
void DepthCameraSensor::UpdateImpl(bool /*_force*/)
{
  //Sensor::Update(force);
  if (this->camera)
  {
    this->camera->Render();
    this->camera->PostRender();
    this->lastUpdateTime = this->world->GetSimTime();
  }
}

void DepthCameraSensor::OnPose(ConstPosePtr &/*_msg*/)
{
}
