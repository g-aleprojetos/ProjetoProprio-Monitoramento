using Microsoft.AspNetCore.Mvc;
using Microsoft.IdentityModel.Tokens;
using Monitoring.Filters;
using Monitoring.Models;
using Monitoring.Models.Users;
using Swashbuckle.AspNetCore.Annotations;
using System;
using System.IdentityModel.Tokens.Jwt;
using System.Security.Claims;
using System.Text;

namespace Monitoring.Controllers
{
    [Route("api/v1/user")]
    [ApiController]
    public class UserController : ControllerBase
    {
        private object _configuration;

        /// <summary>
        /// This service allows you to authenticate a registered and active user
        /// </summary>
        /// <param name="loginViewModelInput"></param>
        /// <returns>Return status ok, user data and token on success</returns>

        [SwaggerResponse(statusCode: 200, description: "Authenticate success", Type = typeof(LoginViewModelInput))]
        [SwaggerResponse(statusCode: 400, description: "Required fields", Type = typeof(ValidateFieldViewModelOutput))]
        [SwaggerResponse(statusCode: 500, description: "internal error", Type = typeof(GenericErrorViewModel))]

        [Route("LogInto")]
        [CustomModelStateValidation]
        [HttpPost]
        public IActionResult LogInto(LoginViewModelInput loginViewModelInput)
        {
            var userViewModelOutput = new UserViewModelOutput()
            {
                Code = 1,
                Login = "Alexandre",
                Email = "g.aleprojetos@gmail.com"
            };

            var secret = Encoding.ASCII.GetBytes("jwtConfiguration:Secret");
            var symmetricSecurityKey = new SymmetricSecurityKey(secret);
            var securityTokenDescriptor = new SecurityTokenDescriptor
    {
        Subject = new ClaimsIdentity(new Claim[]
        {
            new Claim(ClaimTypes.NameIdentifier, userViewModelOutput.Code.ToString()),
            new Claim(ClaimTypes.Name, userViewModelOutput.Login.ToString()),
            new Claim(ClaimTypes.Email, userViewModelOutput.Email.ToString())
        }),
        Expires = DateTime.UtcNow.AddDays(1),
        SigningCredentials = new SigningCredentials(symmetricSecurityKey, SecurityAlgorithms.HmacSha256Signature)
    };

            var jwtSecurityTokenHandler = new JwtSecurityTokenHandler();
            var tokenGenerated = jwtSecurityTokenHandler.CreateToken(securityTokenDescriptor);
            var token = jwtSecurityTokenHandler.WriteToken(tokenGenerated);
          //  var token = _authenticationService.GerarToken(usuarioViewModelOutput);

            return Ok(new
            {
                token = token,
                User = userViewModelOutput
            });
        }

        /// <summary>
        /// This service allows you to register a non-existing user
        /// </summary>
        /// <param name="loginViewModelInput">Login record view model</param>

        [SwaggerResponse(statusCode: 200, description: "Authenticate success", Type = typeof(LoginViewModelInput))]
        [SwaggerResponse(statusCode: 400, description: "Required fields", Type = typeof(ValidateFieldViewModelOutput))]
        [SwaggerResponse(statusCode: 500, description: "internal error", Type = typeof(GenericErrorViewModel))]

        [Route("register")]
        [CustomModelStateValidation]
        [HttpPost]
        public IActionResult Register(LoginViewModelInput loginViewModelInput)
        {
            return Created("", loginViewModelInput);
        }
    }
}
