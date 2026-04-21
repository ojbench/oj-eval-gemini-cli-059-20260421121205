import requests
import json
import os

def submit_cpp(problem_id, file_path, token):
    api_base = "https://acm.sjtu.edu.cn/OnlineJudge/api/v1"
    headers = {
        "Authorization": f"Bearer {token}",
        "Content-Type": "application/x-www-form-urlencoded",
    }
    
    with open(file_path, 'r') as f:
        code = f.read()
    
    data = {
        "language": "cpp",
        "code": code
    }
    
    url = f"{api_base}/problem/{problem_id}/submit"
    response = requests.post(url, headers=headers, data=data, proxies={"https": None, "http": None})
    print(response.text)

if __name__ == "__main__":
    token = os.environ.get("ACMOJ_TOKEN")
    submit_cpp(1440, "/workspace/problem_059/src.hpp", token)
