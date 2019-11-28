import requests
import json
import time
import matplotlib.pyplot as plt


def get_data(uri, format="json"):
    """
        Method description:
        Gets data from the specified container(data_CIN)
        in the OneM2M framework/tree

        Parameters:
        uri : [str] URI for the parent DATA CON appended by "la" or "ol"
        fmt_ex : [str] payload format (json/XML)
    """
    headers = {
        'X-M2M-Origin': 'admin:admin',
        'Content-type': 'application/json'}

    response = requests.get(uri, headers=headers)
    print('Return code : {}'.format(response.status_code))
    print('Return Content : {}'.format(response.text))
    _resp = json.loads(response.text)
    return response.status_code, _resp["m2m:cin"]["con"]


if __name__ == "__main__":
    # server = "http://139.59.42.21:8080"
    server = "http://onem2m.iiit.ac.in:443"
    cse = "/~/in-cse/in-name/"
    ae = "Team37_Soil_Moisture_Sensing/"
    data = get_data(server+cse+ae+"node_1/"+"la")
    print(data)
    data = data[1].split(",")
    y1 = [int(data[0]), int(data[1]), int(data[2]), int(data[3])]
    x1 = [1, 2, 3, 4]
    # y2 = [2000, 1200, 1500, 900]
    # x2 = [1, 2, 3, 4]
    plt.plot(x1, y1, label="Experimental values")
    # plt.plot(x2, y2, label="Expected values")
    plt.xlabel('Sensor')
    plt.ylabel('Moisture Unit (MU)')
    plt.title('Soil Moisture Sensing')
    plt.legend()
    plt.show()
