function temps(kelvin) {
	var celsius = Math.round(kelvin - 273.15);
	var fahrenheit = (celsius * 1.8) + 32;
	return celsius + "\u00B0C/" + fahrenheit + "\u00B0F";
}

function dtime(dt_txt) {
	var dtsplit = dt_txt.split(' ');
	var timesplit = dtsplit[1].split(':');
	return "@" + timesplit[0] + ":" + timesplit[1];
}

function fetchWeather(latitude, longitude) {
  var response;
  var req = new XMLHttpRequest();
  
  var url = "http://api.openweathermap.org/data/2.5/forecast/city?" +
  "lat=" + latitude + "&lon=" + longitude + "&cnt=1";
  
  console.log(url);

  req.open('GET', url, true);
  req.onload = function(e) {
    if (req.readyState == 4) {
      if(req.status == 200) {
        response = JSON.parse(req.responseText);
        var temp1 = '';
        var temp2 = '';
        var temp3 = '';
        var icon1 = '---';
        var icon2 = '---';
        var icon3 = '---';
        var dt1 = '';
        var dt2 = '';
        var dt3 = '';
        if (response && response.list && response.list.length > 0) {
            var weatherResult = response.list[0];
            temp1 = temps(weatherResult.main.temp);
            icon1 = weatherResult.weather[0].icon;
            dt1 = dtime(weatherResult.dt_txt);
            console.log("temp1=" + temp1 + ",icon1=" + icon1 + ", dt1=" + dt1);
            if (response.list.length > 1) {
                weatherResult = response.list[1];
                temp2 = temps(weatherResult.main.temp);
                icon2 = weatherResult.weather[0].icon;
                dt2 = dtime(weatherResult.dt_txt);	
                console.log("temp2=" + temp2 + ",icon2=" + icon2 + ", dt2=" + dt2);
            }
            if (response.list.length > 2) {
                weatherResult = response.list[2];
                temp3 = temps(weatherResult.main.temp);
                icon3 = weatherResult.weather[0].icon;
                dt3 = dtime(weatherResult.dt_txt);	
                console.log("temp3=" + temp3 + ",icon3=" + icon3 + ", dt3=" + dt3);
            }
          Pebble.sendAppMessage({
            "icon1":icon1,
            "temp1":temp1,
            "dt1":dt1,
            "icon2":icon2,
            "temp2":temp2,
            "dt2":dt2,
            "icon3":icon3,
            "temp3":temp3,
            "dt3":dt3});
        }

      } else {
        console.log("Error " + req.status);
      }
    }
  }
  req.send(null);
}

function locationSuccess(pos) {
  var coordinates = pos.coords;
  fetchWeather(coordinates.latitude, coordinates.longitude);
}

function locationError(err) {
  console.warn('location error (' + err.code + '): ' + err.message);
  Pebble.sendAppMessage({
      "icon1":"---",
      "temp1":"",
      "dt1":"",
      "icon2":"---",
      "temp2":"",
      "dt2":"",
      "icon3":"---",
      "temp3":"",
      "dt3":""});
}

var locationOptions = { "timeout": 15000, "maximumAge": 60000 }; 


//Pebble.addEventListener("ready",
//                        function(e) {
//                          console.log("connect!" + e.ready);
//                          locationWatcher = window.navigator.geolocation.watchPosition(locationSuccess, locationError, locationOptions);
//                          console.log(e.type);
//                        });

Pebble.addEventListener("appmessage",
                        function(e) {
                          window.navigator.geolocation.getCurrentPosition(locationSuccess, locationError, locationOptions);
                         });



