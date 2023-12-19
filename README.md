# tdd-template

| Type | Description |
| --- | ----------- |
| L0 | in-memory tests, code in the assembly and nothing else |
| L1 | might require a Database or File System, no deployment |
| L2 | require deployment, might mock some dependencies |
| L3 | full deployment |

Example
[L0](L0), [L0.yml](.github/workflows/L0.yml),
[L1](L1), [L1.yml](.github/workflows/L1.yml),
[workflow runs](https://github.com/npoltorapavlo/tdd-template/actions/)

[Act](https://github.com/nektos/act)\/docker way to run the workflow locally ([PAT](https://github.com/settings/tokens) needed):

```shell script
curl https://raw.githubusercontent.com/nektos/act/master/install.sh | sudo bash
sudo apt update
sudo apt -y install docker.io
sudo bin/act -W .github/workflows/L0.yml -s GITHUB_TOKEN=your-PAT
```
