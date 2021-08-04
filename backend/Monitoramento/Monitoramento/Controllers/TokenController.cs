using Microsoft.AspNetCore.Authorization;
using Microsoft.AspNetCore.Mvc;
using Microsoft.Extensions.Configuration;
using Microsoft.IdentityModel.Tokens;
using Monitoramento.Models;
using System;
using System.IdentityModel.Tokens.Jwt;
using System.Security.Claims;


namespace Monitoramento.Controllers
{
    [Route("api/v1/[controller]")]
    public class TokenController : Controller
    {
        private readonly IConfiguration _configuration;
        public TokenController(IConfiguration configuration)
        {
            _configuration = configuration;
        }

        [AllowAnonymous]
        [HttpPost]
        public IActionResult RequestToken([FromBody] User request)
        {
            if (request.Name == "Mac" && request.Password == "numsey")
            {
                var claims = new[]
                {
                    new Claim(ClaimTypes.Name, request.Name)
                };

                //recebe uma instância da classe SymmetricSecurityKey
                //armazena a chave de criptografia usada na criação do token
                var key = new SymmetricSecurityKey(
                            System.Text.Encoding.UTF8.GetBytes(_configuration["SecurityKey"]));

                //recebe um objeto do tipo SigninCredentials contendo a chava de
                //criptografia e o algoritmo de segurança empregados na geração
                //de assinaturas digitais para tokens 
                var creds = new SigningCredentials(key, SecurityAlgorithms.HmacSha256);

                var token = new JwtSecurityToken(
                    issuer: "g.aleprojetos",
                    audience: "g.aleprojetos",
                    claims: claims,
                    expires: DateTime.Now.AddMinutes(30),
                    signingCredentials: creds);

                return Ok(new
                {
                    token = new JwtSecurityTokenHandler().WriteToken(token)
                });
            }
            return BadRequest("Invalid credentials...");
        }
    }
}
