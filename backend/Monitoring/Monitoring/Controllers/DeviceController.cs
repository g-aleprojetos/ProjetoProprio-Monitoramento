using Microsoft.AspNetCore.Authorization;
using Microsoft.AspNetCore.Http;
using Microsoft.AspNetCore.Mvc;
using Monitoring.Models.Device;
using Swashbuckle.AspNetCore.Annotations;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Security.Claims;
using System.Threading.Tasks;

namespace Monitoring.Controllers
{
    [Route("api/v1/devices")]
    [ApiController]
    [Authorize]
    public class DeviceController : ControllerBase
    {
        /// <summary>
        /// this service allows registering a course for the user to authenticate
        /// </summary>
        /// <returns>Returns 201 status and user course data.</returns>

        [SwaggerResponse(statusCode: 201, description: "Success when registering a device", Type = typeof(DeviceViewModeInput))]
        [SwaggerResponse(statusCode: 401, description: "Not authorized")]

        [HttpPost]
        [Route("")]

        public async Task<IActionResult> Post(DeviceViewModeInput deviceViewModeInput)
        {
            var codeUser = int.Parse(User.FindFirst(c => c.Type == ClaimTypes.NameIdentifier)?.Value);
            return Created("", deviceViewModeInput);
        }

        /// <summary>
        /// This service allows you to get all active courses from the user
        /// </summary>
        /// <returns>Returns status ok and user course data.</returns>

        [SwaggerResponse(statusCode: 200, description: "Authenticate success")]
        [SwaggerResponse(statusCode: 401, description: "Not authorized")]

        [HttpGet]
        [Route("")]
        public async Task<IActionResult> Get()
        {
            var device = new List<DeviceViewModeOutput>();

            device.Add(new DeviceViewModeOutput()
            {
                Login = "",
                Description = "teste",
                Name = "teste"
            });
            return Ok(device);
        }
    }
}
