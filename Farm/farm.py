import requests

class Farm:
    def __init__(self, ip):
        self.ip = ip

        self.makeRequest()

    def makeRequest(self):
        url = "http://localhost:4000/"
        r = requests.post(url + 'add-farm', json={"ip" : self.ip})
        print(r.text)

farm = Farm("192.168.8.01")