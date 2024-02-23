L0 - in-memory tests. L1 database/file system, no deployment. L2 deployment + mock. L3 full deployment.

[Act](https://github.com/nektos/act)\/docker to run the workflow locally ([PAT](https://github.com/settings/tokens) needed):

```shell script
curl https://raw.githubusercontent.com/nektos/act/master/install.sh | sudo bash
sudo apt update
sudo apt -y install docker.io
sudo bin/act -W .github/workflows/L0.yml -s GITHUB_TOKEN=your-PAT
```
