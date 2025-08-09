/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 17:18:51 by jhaddadi          #+#    #+#             */
/*   Updated: 2025/08/09 17:08:37 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// void	handle_cd_command(char *path, int arg_count, t_data *data)
// {
// 	if (arg_count > 2)
// 	{
// 		printf("cd: too many arguments\n");
// 		data->last_status = 1;
// 		return ;
// 	}
// 	while (*path == ' ' || *path == '\t')
// 		path++;
// 	if (*path == '\0' || ft_strcmp(path, "~") == 0)
// 		path = getenv("HOME");
// 	if (chdir(path) != 0)
// 	{
// 		printf("cd: %s: No such file or directory\n", path);
// 		data->last_status = 1;
// 	}
// 	else
// 		data->last_status = 0;
// }

void	handle_cd_command(char **argv, t_data *data)
{
	const char	*target;
	char		*home;

	if (!argv[1])
	{
		home = getenv("HOME");
		if (!home)
		{
			printf("cd: HOME not set\n");
			data->last_status = 1;
			return ;
		}
		target = home;
	}
	else
		target = argv[1];

	if (chdir(target) != 0)
	{
		perror("cd");
		data->last_status = 1;
		return ;
	}
	data->last_status = 0;
}

void	handle_pwd_command(void)
{
	// char *cwd;
	
	// cmd = getcwd(NULL, 0);
	// if (!cwd)
 	// {
	// 	perror("getcwd");
	// 	data->last_status = 1;
	// 	return;
	// }
	// printf("%s\n", cwd);
	// free(cwd);
	// data->last_status = 0;
	char	cwd[1024];

	if (getcwd(cwd, sizeof(cwd)) != NULL)
		printf("%s\n", cwd);
	else
		perror("pwd");
}

void	handle_echo_command(char **argv)
{
	int	i;
	int	newline;

	i = 1;
	newline = 1;
	while (argv[i] && ft_strcmp(argv[i], "-n") == 0)
	{
		newline = 0;
		i++;
	}
	while (argv[i])
	{
		printf("%s", argv[i]);
		if (argv[i + 1])
			printf(" ");
		i++;
	}
	if (newline)
		printf("\n");
}

// void	handle_type_command(const char *input, t_data *data)
// {
// 	char	*path_env;
// 	char	*dir;
// 	char	*full_path;

// 	if (is_shell_builtin(input))
// 	{
// 		printf("%s is builtin\n", input);
// 		return ;
// 	}
// 	path_env = ft_strdup(getenv("PATH"));
// 	if (path_env)
// 	{
// 		dir = ft_strtok(path_env, ":");
// 		while (dir != NULL)
// 		{
// 			full_path = path_join(dir, input);
// 			if (access(full_path, X_OK) == 0)
// 			{
// 				printf("%s is %s\n", input, full_path);
// 				free (path_env);
// 				return ;
// 			}
// 			dir = ft_strtok(NULL, ":");
// 		}
// 		free(path_env);
// 	}
// 	printf("%s: not found\n", input);
// 	data->last_status = 1;
// }
