:grey_exclamation: L0 - in-memory tests, code in the assembly, and nothing else. L1 might require a database or file system, but no deployment. L2 requires deployment and might mock some dependencies. L3 full deployment.

[Act](https://github.com/nektos/act)\/docker to run the workflow locally ([PAT](https://github.com/settings/tokens) needed):

```shell script
curl https://raw.githubusercontent.com/nektos/act/master/install.sh | sudo bash
sudo apt update
sudo apt -y install docker.io
sudo bin/act -W .github/workflows/L0.yml -s GITHUB_TOKEN=your-PAT
```
