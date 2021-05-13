

var btn_Entrar = document.getElementById("btnEntrar");
btn_Entrar.addEventListener("click", function () {

    var login = document.getElementById('login').value;
    var senha = document.getElementById('senha').value;
    var obj;
   

        if((login != "")&&(senha != "")){

            
            alert(login.value + " e " + senha.value); 
            obj = {
                login: login.value,
                senha: senha.value
            };
            console.log(obj.valueOf());
        
            localStorage[login.value] = JSON.stringify(obj);
        
            console.log(JSON.parse(localStorage[login.value]).valueOf());

        }
        else{
            alert("Preencha todos os campos antes de salvar");
        }

})

/*

var login = document.getElementById('login');
var senha = document.getElementById('senha'); 

var obj;

document.getElementById('btnEntrar').addEventListener('click', function(){

    obj = {
        login: login.value,
        senha: senha.value
    };
    console.log(obj.value());

    localStorage[login.value] = JSON.stringify(obj);

    console.log(JSON.parse(localStorage[login.value]).valueOf());
});*/